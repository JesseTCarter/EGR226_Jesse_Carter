/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Interrupt Service Routines Library File

File:           ISR.c

Description:    This is a source file for interrupts, timers, and their
                respective port and pin initializations that has their C
                function declarations, macro definitions, and global variables
                to be shared between several source files.

                    | MSP432 PINS |

           P1.5 -> DC_motor emergency stop button
           P2.0 -> On board red LED
           P2.1 -> On board green LED
           P5.5 -> ADC pin for LCD brightness
           P6.0 -> RGB lights enable/disable
           P7.7 -> PWM pin for LCD brightness

*/

#include <ISR.h>
#include <stdint.h>
#include <stdio.h>
#include "Keypad.h"
#include "LCD.h"
#include "DC_motor.h"
#include "Servo_motor.h"
#include "Timer_A_lights.h"

uint16_t result;
uint8_t flag;

/*

                       | PORT1_IRQHandler function |

        Brief: The PORT1_IRQHandler function is a an interrupt
               service routine, it allows the program to stop
               the DC motor, and return the program to its
               previous state.

        parameters: N/A

        return: N/A

*/

void PORT1_IRQHandler(void)                     //Port 1 interrupt service routine.

{

    if(P1->IFG & BIT5)

        printf("Emergency Motor Stop\n");

            TIMER_A0->CCR[4] = 0;

            P1->IFG &= ~BIT5;

}

/*

                       | PORT6_IRQHandler function |

        Brief: The PORT6_IRQHandler function is a an interrupt
               service routine, it allows the program to turn
               off the RGB LEDS, and turn them back on to their
               previous brightness, then the user is returned back
               to the programs previous state.

        parameters: N/A

        return: N/A

*/

void PORT6_IRQHandler(void)                     //Port 6 interrupt service routine.

{

    if ( P6->IFG & BIT0 )

    {                                           //If P6.0 had an interrupt (going from high to low).

        printf("Toggle LED Interrupt\n");

        if (flag == 0)

        {

            TIMER_A2->CCR[2] = 0;
            TIMER_A2->CCR[3] = 0;
            TIMER_A2->CCR[4] = 0;

            flag=1;

            delay_ms(1);

        }

        else

        {

            switch(num)

            {

            case 1:
                TIMER_A2->CCR[3] = 60000*num/9;
                TIMER_A2->CCR[2] = 0;
                TIMER_A2->CCR[4] = 0;
                delay_ms(1);
                break;

            case 2:
                TIMER_A2->CCR[2] = 60000*num/9;
                TIMER_A2->CCR[3] = 0;
                TIMER_A2->CCR[4] = 0;
                delay_ms(1);
                break;

            case 3:
                TIMER_A2->CCR[4] = 60000*num/9;
                TIMER_A2->CCR[3] = 0;
                TIMER_A2->CCR[2] = 0;
                delay_ms(1);
                break;

            }

            flag = 0;

            delay_ms(1);

        }

    }

    P6->IFG &= ~BIT0;               //Reset the interrupt flag.

    delay_ms(1);

}

/*
                       | IRQ_pin_init function |

        Brief: The IRQ_pin_init function initializes the required
               ports and pins on the MSP432 for the programs used
               timers and interrupts.

        parameters: N/A

        return: N/A

*/

void IRQ_pin_init(void)

{

    P1 -> SEL0 &= ~BIT5;            //Sets motor emergency stop button.
    P1 -> SEL1 &= ~BIT5;
    P1 -> DIR &= ~BIT5;
    P1 -> REN |= BIT5;              //Enable internal resistor.
    P1 -> OUT |= BIT5;              //Enable pull up resistor.
    P1 -> IES |= BIT5;              //Set pin interrupt to trigger when it goes high to low.
    P1 -> IE |= BIT5;               //Enable interrupt.
    P1 -> IFG =0;                   //Clear all P1 interrupts.

    P6 -> SEL0 &= ~BIT0;            //Sets motor emergency stop button.
    P6 -> SEL1 &= ~BIT0;
    P6 -> DIR &= ~BIT0;
    P6 -> REN |= BIT0;              //Enable internal resistor.
    P6 -> OUT |= BIT0;              //Enable pull up resistor.
    P6 -> IES |= BIT0;              //Set pin interrupt to trigger when it goes high to low.
    P6 -> IE |= BIT0;               //Enable interrupt.
    P6 -> IFG = 0;                  //Clear all P6 6nterrupts.

    P2 -> SEL1 &= ~BIT0 | BIT1;
    P2 -> SEL0 &= ~BIT0 | BIT1;
    P2 -> DIR  |=  BIT0 | BIT1;

    P7 -> SEL1 &= ~BIT7;            //Configure P7.7 as simple I/O Output.
    P7 -> SEL0 |= BIT7;
    P7 -> DIR |= BIT7;

}

/*
                       | ADC_init function |

        Brief: ADC_init will initialize the analog input channel A0 using
               port 5 pin 5 which is set to use analog-to-digital conversion
               (ADC). ADC is in 14-bit resolution with the 1/4 clock divider,
               S/H pulse mode, SW trigger enabled, MCLCK, and 32 sample clock.

        parameters: N/A

        return: N/A

*/

void ADC_init(void)

{

    ADC14->CTL0 = 0x00000010;       //Power on and disabled during configuration.
    ADC14->CTL0 |= 0x04D80300;      //S/H pulse mode, MCLCK, 32 Sample clocks sw trigger, /4 clock divider.
    ADC14->CTL1 = 0x00000030;       //14-bit resolution.
    ADC14->MCTL[5] = 0;             //A0 input, single ended, vref=avcc.
    P5->SEL1 |= 0x20;               //Configure pin 5.5 for A0.
    P5->SEL0 |= 0x20;
    ADC14->CTL1 |= 0x00050000;      //Start converting at mem reg 5.
    ADC14->CTL0 |= 2;               //Enable ADC after configuration.

}

/*
                       | T32_INT1_IRQHandler function |

        Brief: The T32_INT1_IRQHandler function initializes the
               required Timer_32 handler that will use ADC conversion.

        parameters: N/A

        return: N/A

*/

void T32_INT1_IRQHandler(void)

{

    ADC14->CTL0 |= 1;               //Start a conversion.
    while(!ADC14->IFGR0);           //Wait until conversion is completed.
    result = ADC14->MEM[5];         //Immediately store value into a variable.
    result = (result*2.441);
    TIMER_A1->CCR[1] = result;
    TIMER32_1->INTCLR = 0x0;

}

/*

                       | Timer_32_init function |

        Brief: Timer_32_init initializes all of the
               corresponding timer CONTROL, LOAD, and
               INTCLR registers.

        parameters: N/A

        return: N/A

*/


void Timer_32_init(void)

{

    TIMER32_1 -> CONTROL = 0xE6;
    TIMER32_1 -> LOAD = 46875-1;
    TIMER32_1 -> INTCLR = 0x0;

}

/*

                       | Timer_A_LCD_init function |

        Brief: Timer_A_LCD_init initializes all of the
               corresponding timer CCR, CCTL, and
               CTL registers.

        parameters: N/A

        return: N/A

*/

void Timer_A_LCD_init(void)

{

    TIMER_A1->CCR[0]  = 37500;                      //Sets the period of the timer.
    TIMER_A1->CTL = 0b0000001000010100;             //Clock divider is set to /1.
    TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //Sets the mode.
    TIMER_A1->CCR[1] = 0;

}
