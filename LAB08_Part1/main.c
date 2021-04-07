/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/17/2021

Project:        LAB08_Part1

File:           mainPart1.c

Professor:      Trevor Ekin

Description:    This program controls the DC motor using the SysTick Timer.
                The SysTick timer is used to generate the PWM mode to create
                an analog signal for controlling the speed of the motor.

*/

#include "msp.h"
#include <stdio.h>
#include <math.h>

void SysTick_init (void);       //Prototype function for SysTick initialization.
void SysTick_delay(uint16_t delay);       //Prototype function for delays in milliseconds.
void Pin_init(void);        //Prototype function for pin initialization.

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    SysTick_init();     //SysTick_init function being called.

    Pin_init();     //Pin_init function being called.

    int i = 50;

    int timeon;

    int timeoff;

    while(1)

    {

        P2OUT |= BIT4;      //Turns motor on.

        SysTick_delay(timeon);

        P2OUT &= ~BIT4;     //Turns motor off.

        SysTick_delay(timeoff);

        timeon = 25 * i * .01;

        timeoff = 25 - timeon;


    }

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

        parameters: uint16_t delay

        return: N/A

*/

void SysTick_delay(uint16_t delay)

{

    SysTick-> LOAD = ((delay * 3000) - 1);      //Delay of 1ms per delay value.

    SysTick-> VAL = 0;      //Any write to CVR clears it.

    while((SysTick-> CTRL & 0x00010000) == 0);      //Wait for flag to be SET.

}

/*
                       | Pin_inst function |

        Brief: The pin_inst function initializes the required
               ports and pins on the MSP432 for the program.

        parameters: N/A

        return: N/A

*/

void Pin_init(void)

{

    P2->SEL1 &= ~BIT4;      //Configure P2.4 as input/output.

    P2->SEL0 &= ~BIT4;

    P2->DIR |= BIT4;

    P2->OUT &= ~BIT4;       //Set P2.4 as an output pin.

}
