/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/24/2021

Project:        LAB09_Part1

File:           mainPart1.c

Professor:      Trevor Ekin

Description:    This program the pulse-width modulation (PWM) is generated using Timer_A#
                which will create a PWM signal that is connected to an input/output pin
                for controlling the speed of the DC motor. When button 1 is pressed the speed
                of the motor is increased by 10%, when button 2 is pressed, the speed of the
                DC motor is decreased by 10%, and when button 3 is pressed, the DC motor turns off.

*/

#include "msp.h"

void Pin_init(void);        //Prototype function for pin initialization.
void TimerA_init(void);     //Prototype function for Timer_A# initialization.
void Switch_init(void);     //Prototype function for the 3 push buttons initialization.
void SysTick_init(void);        //Prototype function for SysTick initialization.
void SysTick_delay(uint16_t Delay);     //Prototype function for delays in milliseconds.
uint8_t DebounceSwitch1(void);      //Prototype function for checking the switch-bounce on button 1.
uint8_t DebounceSwitch2(void);      //Prototype function for checking the switch-bounce on button 2.
uint8_t DebounceSwitch3(void);      //Prototype function for checking the switch-bounce on button 3.

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    Pin_init();     //Pin_init function being called.

    TimerA_init();      //TimerA_init function being called.

    Switch_init();      //Switch_init function being called.

    SysTick_init();     //SysTick_init function being called.

    while(1)

    {

        if(DebounceSwitch1())       //When button 1 is pressed speed increases by 10%.

        {

            TIMER_A0->CCR[1] = TIMER_A0->CCR[1] + (937.5*10)/(100);

        }

        if(DebounceSwitch2())       //When button 2 is pressed speed decreases by 10%.

        {

            TIMER_A0->CCR[1] = TIMER_A0->CCR[1] - (937.5*10)/(100);

        }

        if(DebounceSwitch3())       //When button 3 is pressed the DC motor turns off.

        {

            TIMER_A0->CCR[1] = 0;

        }

    }

}

/*
                       | Pin_init function |

        Brief: The Pin_init function initializes the required
               ports and pins on the MSP432 for the program.

        parameters: N/A

        return: N/A

*/

void Pin_init(void)

{

    P2->SEL1 &= ~BIT4;      //Configuring port 2 pin 4 as GPIO for pulse-width modulation.

    P2->SEL0 |= BIT4;       //Port 2 pin 4 set to TimerA_0.1.

    P2->DIR |= BIT4;

}

/*
                       | TimerA_init function |

        Brief: This function initializes the Timer A
               peripherals to create a PWM on DC motor

        parameters: N/A

        return: N/A
*/

void TimerA_init(void)

{

    TIMER_A0->CCR[0] = 9375;        //Pulse-width modulation period, number cycle of the clock.

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //CCTL reset/set mode 7.

    TIMER_A0->CCR[1] = 0;       //CCR1 pulse-width modulation duty cycle.

    TIMER_A0->CTL = 0b0000001011010100;     //Clock divider 1/8.

}

/*
                       | Switch_init function |

        Brief: The Switch_init function initializes the proper
               ports and pins for the 3 external push button
               switches that have pull-up resistors enabled.

        parameters: N/A

        return: N/A
*/

void Switch_init(void)

{

    P2->SEL1 &= ~(BIT5 | BIT6 | BIT7);           //Configuring port 2; pins 5, 6, and 7, as input/output.

    P2->SEL0 &= ~(BIT5 | BIT6 | BIT7);

    P2->DIR &= ~(BIT5 | BIT6 | BIT7);            //Port 2; pins 5, 6, and 7, set as input.

    P2->REN |= (BIT5 | BIT6 | BIT7);             //Port 2; pins 5, 6, and 7, pull resistor enabled.

    P2->OUT |= (BIT5 | BIT6 | BIT7);             //Port 2; pins 5, 6, and 7, pull up/down resistor is selected.

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

        Brief: SysTick_delay runs the delay in milliseconds when
               system clock is at 3MHz.

        parameters: int n

        return: N/A

*/

void SysTick_delay(uint16_t delay)

{

    SysTick-> LOAD = ((delay * 3000) - 1);      //Delay of 1ms per delay value.

    SysTick-> VAL = 0;      //Any write to CVR clears it.

    while((SysTick-> CTRL & 0x00010000) == 0);      //Wait for flag to be SET.

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

uint8_t DebounceSwitch1(void)       //Red button switch on port 2 pin 5.

{

    static uint16_t State = 0;

    State = (State<<1) | ((P2IN & BIT5)>>1) | 0xfc00;

    SysTick_delay(10);      //10 millisecond delay for switch bounce.

    if(State == 0xfc00)

        return 1;

    return 0;

}

/*
                       | DebounceSwitch2 function |

        Brief: The DebounceSwitch2 function uses the internal SysTick
               timer to manage time to solve for the switch bounce
               An if loop checks if a push-button is pressed, if
               the button is pressed, a 1 is returned, otherwise a 0
               is returned (This is for the programmed switch-button two).

        parameters: N/A

        return: uint8_t

*/

uint8_t DebounceSwitch2(void)       //Green button switch on port 2 pin 6.

{

    static uint16_t State = 0;

    State = (State<<1) | ((P2IN & BIT6)>>1) | 0xfc00;

    SysTick_delay(10);      //10 millisecond delay for switch bounce.

    if(State == 0xfc00)

        return 1;

    return 0;

}

/*
                       | DebounceSwitch3 function |

        Brief: The DebounceSwitch3 function uses the internal SysTick
               timer to manage time to solve for the switch bounce
               An if loop checks if a push-button is pressed, if
               the button is pressed, a 1 is returned, otherwise a 0
               is returned (This is for the programmed switch-button three).

        parameters: N/A

        return: uint8_t

*/

uint8_t DebounceSwitch3(void)       //Blue button switch on port 2 pin 7.

{

    static uint16_t State = 0;

    State = (State<<1) | ((P2IN & BIT7)>>1) | 0xfc00;

    SysTick_delay(10);      //10 millisecond delay for switch bounce.

    if(State == 0xfc00)

        return 1;

    return 0;

}
