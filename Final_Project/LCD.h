/*

Name:           Nabeeh Kandalaft

Course:         EGR 226

Date:           September-15-2018

Project:        LCD Header File

File:           LCD.h

Description:    This is a header file for the 4x16 LCD that
                contains C function declarations, macro definitions,
                and global variables to be shared between several source files.

          | MSP432 PINS |

           P4.0 -> LCD D4
           P4.1 -> LCD D5
           P4.2 -> LCD D6
           P4.3 -> LCD D7
           P4.4 -> LCD E
           P4.5 -> LCD RS

*/

#ifndef LCD_H_
#define LCD_H_

//#include "driverlib.h"
//For use with driverlib.

#include "msp.h"
#include <stdint.h>

#define EN      BIT4      //Pin  P4.4    on any PORT.
#define RS      BIT5      //Pin  P4.5    on any PORT.
#define DATA    0x0F      //Pins P4.0-3  on any PORT.

/*

         | Commands |

*/

#define CLEAR   0x01

/*

         | Functions |

*/

void SysTick_initialization(void);
void delay_ms(volatile uint32_t ms_delay);
void delay_us(volatile uint32_t us_delay);
void lcdInit ();                                //Clear LCD.
void lcdClear();                                //Initialize LCD.
void lcdTriggerEN();                            //Trigger enable.
void lcdWriteData(unsigned char data);          //Send data (Characters).
void lcdWriteCmd (unsigned char  cmd);          //Send commands.
void lcdSetText(char * text, int x, int y);     //Write string.
void lcdSetInt (int val,     int x, int y);     //Write integer.
void Door_Menu();
void Motor_Menu();
void Lights_Menu();
void Main_Menu();

#endif /* LCD_H_ */
