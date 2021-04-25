/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Timer_A_lights Header File

File:           Timer_A_lights.h

Description:    This is a header file for the integrated RGB lights
                that contains its C function declarations for the RGB lights
                timers, and port and pin initializations that will be shared
                between several source files.

                | MSP432 PINS |

                P5.7 -> Green LED
                P6.6 -> Red LED
                P6.7 -> Blue LED

*/

#ifndef TIMER_A_LIGHTS_H_
#define TIMER_A_LIGHTS_H_

//#include "driverlib.h"
//For use with driverlib.

#include "msp.h"
#include <stdint.h>

/*

         | Functions |

*/

void Timer_A_lights_init(void);
void Lights_init(void);

#endif /* TIMER_A_LIGHTS_H_ */
