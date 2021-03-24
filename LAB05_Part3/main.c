/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           02/24/2021

Project:        LAB05_Part3

File:           mainPart3.c

Professor:      Trevor Ekin

Description:    A program that checks if one of two buttons are pressed, if the white button
                is pressed and held, a green LED turns on and sequences through three different
                led colors, green, yellow, red. If the black button is pressed and held
                the original sequence happens in reverse, starting from wherever the current LED is lit.
                Incorporates a de-bounce switch function to solve for the switch bounce,
                so that the button presses are accurate with changing the LED colors, so
                that no interference accidentally changes which LED is lit.

*/

#include "msp.h"
#include "stdio.h"

void pin_init(void);        //Prototype function for pin initialization.
void SysTick_init(void);        //Prototype function for SysTick initialization.
void SysTick_delay(uint16_t delay);     //Prototype function for setting up SysTick delay value.
uint8_t DebounceSwitch1(void);      //Prototype function for checking for De-bounce on switch 1.
uint8_t DebounceSwitch2(void);      //Prototype function for checking for De-bounce on switch 2.

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    SysTick_init();     //Calls the function that initializes SysTick.

    pin_init();        //Calls the function that initializes the ports and pins.

    int color = 4;

    while(1)

    {

        while(DebounceSwitch1())

        {

            if(color >= 8) color = 5;

            color++;

            P3OUT &= BIT(color);            //Clear Port 3 Bit color.
            P3OUT ^= BIT(color);            //Toggle Port 3 Bit color.

            //printf("%d\n", color);        //Used to help debug.

            if(color >= 7) color = 4;       //Sets color back to 4.

            SysTick_delay(1000);        //SysTick delay for 1 second.

        }

        while(DebounceSwitch2())

        {

            if(color <=4 ) color = 7;

            color--;

            P3OUT &= BIT(color);        //Clear Port 3 Bit color.
            P3OUT ^= BIT(color);        //Toggle Port 3 Bit color.

            //printf("%d\n", color);        //Used to help debug.

            if(color <= 5) color = 8;       //Sets color back to 8.

            SysTick_delay(1000);        //SisTick delay for 1 second.

        }

    }

}

/*
                       | DebounceSwitch1 function |

        Brief: The DebounceSwitch1 function uses the internal SysTick
               timer to manage time to solve for the switch bounce
               An if loop checks if a push-button is pressed, if
               the button is pressed, a 1 is returned, otherwise a 0
               is returned (This is for the programmed switch-button one).

        parameters: N/A

        return: uint8_t

*/

uint8_t DebounceSwitch1(void)       //White button switch on Port P2.5.

{

    static uint16_t State = 0;

    State = (State<<1) | ((P2IN & BIT5)>>5) | 0xfc00;

    SysTick_delay(5);       //5 millisecond delay for switch bounce.

    if(State == 0xfc00)

        return 1;

    return 0;

}

/*
                       | DebounceSwitch2 function |

        Brief: The DebounceSwitch1 function uses the internal SysTick
               timer to manage time to solve for the switch bounce
               An if loop checks if a push-button is pressed, if
               the button is pressed, a 1 is returned, otherwise a 0
               is returned (This is for the programmed switch-button two).

        parameters: N/A

        return: uint8_t

*/

uint8_t DebounceSwitch2(void)       //Black button switch on Port P2.6.

{

    static uint16_t State = 0;

    State = (State<<1) | ((P2IN & BIT6)>>6) | 0xfc00;

    SysTick_delay(5);       //5 millisecond delay for switch bounce.

    if(State == 0xfc00)

        return 1;

    return 0;

}

/*
                       | pin_inst function |

        Brief: The pin_inst function initializes the required
               ports and pins on the MSP432 for the program.

        parameters: N/A

        return: N/A

*/

void pin_init(void)

{

    P2->SEL1 &= ~BIT5;      //Configure P2.5 as input/output.
    P2->SEL0 &= ~BIT5;
    P2->DIR &= ~BIT5;       //P2.5 set as an input.
    P2->REN |= BIT5;        //P2.5 pull resistor enabled.
    P2->OUT |= BIT5;        //Pull up/down resister is selected by P2OUT.

    P2->SEL1 &= ~BIT6;      //Configure P2.6 as input/output.
    P2->SEL0 &= ~BIT6;
    P2->DIR &= ~BIT6;       //P2.6 set as input.
    P2->REN |= BIT6;        //P2.6 pull resistor enabled.
    P2->OUT |= BIT6;        //Pull up/down resistor is selected by P2OUT.

    P3->SEL0 &= ~BIT5;      //Configure P3.5 as input/output.
    P3->SEL1 &= ~BIT5;
    P3->DIR |= BIT5;        //Set P3.5 as an output pin.
    P3->OUT &= BIT5;

    P3->SEL0 &= ~BIT6;      //Configure P3.6 as input/output.
    P3->SEL1 &= ~BIT6;
    P3->DIR |= BIT6;        //Set P3.6 as an output pin.
    P3->OUT &= BIT6;

    P3->SEL0 &= ~BIT7;      //Configure P3.7 as input/output.
    P3->SEL1 &= ~BIT7;
    P3->DIR |= BIT7;        //Set P3.7 as an output pin.
    P3->OUT &= BIT7;

}

/*
                       | SysTick_init function |

        Brief: The SysTick_init function initializes the MSP432's
               built-in internal timer for more accurate delays.

        parameters: N/A

        return: N/A

*/

void SysTick_init(void)

{

    SysTick-> CTRL = 0;     //Disable SysTick during step.
    SysTick-> LOAD = 0x00FFFFFF;        //Max reload value.
    SysTick-> VAL = 0;      //Any write to CVR clears it.
    SysTick-> CTRL = 0x00000005;        //Enable SysTick, 3MHz, no interrupts.

}

/*
                       | SysTick_delay function |

        Brief: The SysTick_delay function sets up the SysTick
               timer delay value, it's setup for 1ms delays
               per 1 value of SysTick_delay.

        parameters: uint16_t delay

        return: N/A

*/

void SysTick_delay(uint16_t delay)

{

    SysTick-> LOAD = ((delay*3000)-1);      //Delay for 1ms per delay value.
    SysTick-> VAL = 0;      //Any write to CVR clears it.
    while((SysTick-> CTRL & 0x00010000) == 0);      //Wait for flag to be SET.

}
