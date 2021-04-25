/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        DC_motor Source File

File:           DC_motor.c

Description:    This is a source file for the integrated DC motor
                that contains its C function code for the motors
                timer, and port and pin initializations that will be shared
                between several source files.

                | MSP432 PINS |

                P2.7 -> DC_motor

*/

#include <DC_motor.h>
#include <stdio.h>
#include <stdint.h>
#include "Keypad.h"
#include "LCD.h"
#include "Servo_motor.h"
#include "Timer_A_lights.h"
#include "ISR.h"

/*

                       | Timer_A_init function |

        Brief: Timer_A_init initializes all of the
               corresponding timer CCR, CCTL, and
               CTL registers.

        parameters: N/A

        return: N/A

*/

void Timer_A_init(void)

{

    TIMER_A0->CCR[0]  = 9375;                        //Sets the period of the motor.
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;      //Sets the mode.
    TIMER_A0->CCR[4] = 0;
    TIMER_A0->CTL = 0b0000001011010100;              //Clock divider is set to 1/8.

}

/*
                       | Motor_init function |

        Brief: The Motor_init function initializes the required
               ports and pins on the MSP432 for the DC_motor.

        parameters: N/A

        return: N/A

*/

void Motor_init(void)

{

    P2->SEL1 &= ~BIT7;                               //Configure P2.4 as simple I/O. (Output)
    P2->SEL0 |= BIT7;
    P2->DIR |= BIT7;

}
