/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Keypad Source File

File:           Keypad.c

Description:    This is a header file for the integrated keypad
                that contains its C function declarations, macro
                definitions, and global variables to be shared
                between several source files.

          | MSP432 PINS |

        P2.3 -> KeyPad ROW 1
        P2.4 -> KeyPad ROW 2
        P2.5 -> KeyPad ROW 3
        P2.6 -> KeyPad ROW 4
        P3.5 -> KeyPad COL 1
        P3.6 -> KeyPad COL 2
        P3.7 -> KeyPad COL 3

*/

#ifndef KEYPAD_H_
#define KEYPAD_H_

//#include "driverlib.h"
//For use with driverlib.

#include "msp.h"
#include <stdint.h>

/*

         | Functions |

*/

void Keypad_init(void);
void Print_Main_Menu(void);
void Print_Door_Menu(void);
void Print_Lights_Menu(void);
void Get_Motor_Speed(void);
void Red_brightness(void);
void Blue_brightness(void);
void Green_brightness(void);
void Menu_Loop(void);
uint8_t Keypad_read(void);

/*

         | Global Variables |

*/

uint8_t num, pressed;



#endif /* KEYPAD_H_ */
