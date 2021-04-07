/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/07/2021

Project:        LAB11_Part1

File:           mainPart1.c

Professor:      Trevor Ekin

Description:    Part 1 is programmed to generate an IR signal and
                is to be used to verify that the circuit is built
                correctly.

*/

#include "msp.h"

void TimerA0Init(void);     //Prototype function for TimerA register initialization.
void Pin_init(void);        //Prototype function for pin initialization.

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    Pin_init();

    TimerA0Init();

    NVIC->ISER[0] = 1<<((TA2_N_IRQn) & 31);     //Enable interrupt in NVIC vector.

    __enable_irq();     //Enable global interrupt.

    while(1)

        {

            ;

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

    P2->SEL0 |= BIT4;       //P2.4 set TA0.1

    P2->SEL1 &= ~BIT4;

    P2->DIR |= BIT4;

}

/*

                       | TimerA0Init function |

        Brief: TimerA0Init initializes all of the
               corresponding timer CCR, CCTL, and
               CTL registers.

        parameters: N/A

        return: N/A

*/


void TimerA0Init(void)

{


    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK|        //SMCLK.

            TIMER_A_CTL_MC__UP|     //Up mode.

            TIMER_A_CTL_CLR|        //Clear timer.

            TIMER_A_CTL_ID__8;      //Divide clock by 8.

    TIMER_A0->CCR[0] = 37500;       //10 Hz.

    TIMER_A0->CCR[1] = 37500 / 2;       //50% duty cycle.

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

}
