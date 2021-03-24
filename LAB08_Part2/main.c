/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/17/2021

Project:        LAB08_Part2

File:           mainPart2.c

Professor:      Trevor Ekin

Description:    This program controls the DC motor using Timer_A. The program
                uses Timer_A to generate the PWM mode to create an analog signal
                for controlling the speed of the motor.

*/

#include "msp.h"
#include <stdio.h>
#include <math.h>

void Pin_init(void);        //Prototype function for pin initialization.
void TimerA_init(void);     //Prototype function for Timer_A# initialization.

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    int i = 3000;

    Pin_init();     //Pin_init function being called.

    TimerA_init();      //TimerA_init function being called.

    while(1)

    {

        TIMER_A0->CCR[1] = i;

    }


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

    P2->SEL0 |= BIT4;

    P2->DIR |= BIT4;

}

/*
                       | TimerA_init function |

        Brief: This function initializes the Timer_A#
               peripherals to generate a PWM on a DC motor

        parameters: N/A

        return: N/A
*/

void TimerA_init(void)

{

    TIMER_A0->CCR[0] = 9375;

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    TIMER_A0->CCR[1] = 0;       //Changes the value of the motor speed using duty cycle.

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK| TIMER_A_CTL_MC__UP|TIMER_A_CTL_CLR;

}

