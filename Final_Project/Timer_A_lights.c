/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Timer_A_lights Source File

File:           Timer_A_lights.c

Description:    This is a source file for the integrated RGB lights
                that contains its C function code for the RGB lights
                timers, and port and pin initializations that will be shared
                between several source files.

                | MSP432 PINS |

                P5.7 -> Green LED
                P6.6 -> Red LED
                P6.7 -> Blue LED

*/

#include <Timer_A_lights.h>
#include <stdio.h>
#include <stdint.h>
#include "Keypad.h"
#include "LCD.h"
#include "DC_motor.h"
#include "Servo_motor.h"
#include "ISR.h"

/*

                       | Timer_A_lights_init function |

        Brief: Timer_A_lights_init initializes all of the
               corresponding timers CCR, CCTL, and
               CTL registers for the RGB lights.

        parameters: N/A

        return: N/A

*/

void Timer_A_lights_init(void)

{

    TIMER_A2->CCR[0]  = 60000;                      //Sets the period of the RGB lights.
    TIMER_A2->CTL = 0b0000001000010100;             //Clock divider is set to 1/1.

    TIMER_A2->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;     //Sets the mode.
    TIMER_A2->CCR[2] = 0;

    TIMER_A2->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;     //Sets the mode.
    TIMER_A2->CCR[3] = 0;

    TIMER_A2->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;     //Sets the mode.
    TIMER_A2->CCR[4] = 0;

}

/*
                       | Lights_init function |

        Brief: The Lights_init function initializes the required
               ports and pins on the MSP432 for the RGB lights.

        parameters: N/A

        return: N/A

*/

void Lights_init(void)

{

    P5->SEL1 &= ~BIT7;
    P5->SEL0 |= BIT7;
    P5->DIR |= BIT7;

    P6->SEL1 &= ~BIT6;
    P6->SEL0 |= BIT6;
    P6->DIR |= BIT6;

    P6->SEL1 &= ~BIT7;
    P6->SEL0 |= BIT7;
    P6->DIR |= BIT7;

}
