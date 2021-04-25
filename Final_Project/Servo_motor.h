/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Servo_motor Header File

File:           Servo_motor.h

Description:    This is a header file for the integrated servo motor
                that contains its C function declarations for the motors
                timer, and port and pin initializations that will be shared
                between several source files.

          | MSP432 PINS |

        P5.6 -> Servo_motor

*/

#ifndef SERVO_MOTOR_H_
#define SERVO_MOTOR_H_

//#include "driverlib.h"
//For use with driverlib.

#include "msp.h"
#include <stdint.h>

/*

         | Functions |

*/

void Timer_A_servo_init(void);
void Servo_motor_init(void);

#endif /* SERVO_MOTOR_H_ */
