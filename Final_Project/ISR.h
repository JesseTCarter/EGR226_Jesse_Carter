/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Interrupt Service Routines Header File

File:           ISR.h

Description:    This is a header file for interrupts, timers, and their
                respective port and pin initializations that has their C
                function declarations, macro definitions, and global variables
                to be shared between several source files.

                    | MSP432 PINS |

           P1.5 -> DC_motor emergency stop button
           P2.0 -> On board red LED
           P2.1 -> On board green LED
           P5.5 -> ADC pin for LCD brightness
           P6.0 -> RGB lights enable/disable
           P7.7 -> PWM pin for LCD brightness

*/

#ifndef ISR_H_
#define ISR_H_

//#include "driverlib.h"
//For use with driverlib.

#include "msp.h"
#include <stdint.h>

/*

         | Functions |

*/

void PORT1_IRQHandler(void);
void PORT6_IRQHandler(void);
void IRQ_pin_init(void);
void T32_INT1_IRQHandler(void);
void Timer_A_LCD_init(void);
void ADC_init(void);
void Timer_32_init(void);

/*

         | Global Variables |

*/

uint16_t result;
uint8_t flag;

#endif /* ISR_H_ */
