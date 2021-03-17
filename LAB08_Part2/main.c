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

void pin_init(void);

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    int i = 300000

    pin_init();

    while(1)

    {

     ;

    }


}

void pin_init(void)

{

    P2->SEL1 &= ~BIT4;      //Configure P2.4 as input/output.

    P2->SEL0 |= BIT4;

    P2->DIR |= BIT4;

    TIMER_A0->CCR[0] = 600000;

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    TIMER_A0->CCR[1] = i;       //Change value for motor speed

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK| TIMER_A_CTL_MC__UP|TIMER_A_CTL_CLR;

}
