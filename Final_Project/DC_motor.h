/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        DC_motor Header File

File:           DC_motor.h

Description:    This is a header file for the integrated DC motor
                that contains its C function declarations for the motors
                timer, and port and pin initializations that will be shared
                between several source files.

                | MSP432 PINS |

                P2.7 -> DC_Motor

*/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

//#include "driverlib.h"
//For use with driverlib

#include "msp.h"
#include <stdint.h>

/*

         | Functions |

*/

void Timer_A_init(void);
void Motor_init(void);

#endif /* DC_MOTOR_H_ */

