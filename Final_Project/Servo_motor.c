/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Servo_motor Source File

File:           Servo_motor.c

Description:    This is a source file for the integrated servo motor
                that contains its C function code for the motors
                timer, and port and pin initializations that will be shared
                between several source files.

          | MSP432 PINS |

        P5.6 -> Servo_motor

*/

#include <Servo_motor.h>
#include <stdio.h>
#include <stdint.h>
#include "LCD.h"
#include "Keypad.h"
#include "DC_motor.h"
#include "Timer_A_lights.h"
#include "ISR.h"

/*

                       | Timer_A_servo_init function |

        Brief: Timer_A_servo_init initializes all of the
               corresponding timer CCR, CCTL, and
               CTL registers.

        parameters: N/A

        return: N/A

*/

void Timer_A_servo_init(void)

{

    TIMER_A2->CCR[0]  = 60000;                       //Sets the period of the motor.
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;      //Sets the mode.
    TIMER_A2->CCR[1] = 0;
    TIMER_A2->CTL = 0b0000001000010100;              //Clock divider is set to 1/1.

}

/*
                       | Servo_motor_init function |

        Brief: The Servo_motor_init function initializes the required
               ports and pins on the MSP432 for the servo_motor.

        parameters: N/A

        return: N/A

*/

void Servo_motor_init(void)

{

    P5->SEL1 &= ~BIT6;                               //Configure P2.4 as simple I/O. (Output)
    P5->SEL0 |= BIT6;
    P5->DIR |= BIT6;

}
