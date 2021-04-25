/*

Name:           Nabeeh Kandalaft

Course:         EGR 226

Date:           September-15-2018

Project:        LCD Library

File:           LCD.c

Description:    This is a library for the 4x16 LCD.
                The lcdSetText(char * text, int x, int y); allows the
                user to print where X is the displacement from the beginning
                of the line Y is the line number. The lcdSetInt(int val, int x, int y);
                Convert the integer to character string and print the string on
                the LCD where X, Y are displacement and the line number.

                        | MSP432 PINS |

                         P4.0 -> LCD D4
                         P4.1 -> LCD D5
                         P4.2 -> LCD D6
                         P4.3 -> LCD D7
                         P4.4 -> LCD E
                         P4.5 -> LCD RS

*/

#include <LCD.h>
#include <stdint.h>
#include <stdio.h>
#include "Keypad.h"
#include "DC_motor.h"
#include "Servo_motor.h"
#include "Timer_A_lights.h"
#include "ISR.h"

#define Lower_Nibble(x)   P4->OUT = (P4->OUT & 0xF0) + (x & 0x0F)       //This macro if PORT(x) uses the lower 4 pins.

/*

                    if PORT(x) uses the Upper 4 pins, you can use
                    #define Upper_Nibble(x) P4->OUT = (P4->OUT & 0x0F) + (x & 0xF0)<<4.
                    Make sure to change the Lower_Nibble to Upper_Nibble in LcdWriteCmd and LcdWritedata functions.
                    Also change the pins for Enable and RS command according to your pin selection.

*/

/*
                       | SysTick_initialization |

        Brief: The SysTick_init function initializes the MSP432's
               built-in internal timer for more accurate delays.

        parameters: N/A

        return: N/A

*/

void SysTick_initialization(void)

{

    SysTick->CTRL  &= ~BIT0;            //Clears enable to stop the counter.
    SysTick->LOAD  = 0x00FFFFFF;        //Sets the period. (Note: (3006600/1000 - 1) = 1ms)
    SysTick->VAL   = 0;                 //Clears the value.
    SysTick->CTRL  = 0x00000005;        //Enable SysTick, no uint8_terrupts.

}

/*
                       | delay_ms function |

        Brief: The delay_ms function sets up the SysTick
               timer delay value, it's setup for 1ms delays
               per 1 value of delay_ms.

        parameters: uint32_t ms_delay

        return: N/A

*/

void delay_ms(uint32_t ms_delay)

{

    SysTick->LOAD = ms_delay*3000 - 1;             //Counts up to delay.
    SysTick->VAL  = 0;                             //Starts counting from 0.
    while((SysTick->CTRL && 0x00010000) == 0);     //Wait until flag is set. (Delay number is reached)

}

/*
                       | delay_us function |

        Brief: The delay_us function sets up the SysTick
               timer delay value, it's setup for 1us delays
               per 1 value of delay_us.

        parameters: uint32_t us_delay

        return: N/A

*/

void delay_us(uint32_t us_delay)

{

    SysTick->LOAD = us_delay*3 - 1;                //Counts up to delay.
    SysTick->VAL  = 0;                             //Starts counting from 0.
    while((SysTick->CTRL && 0x00010000) == 0);     //Wait until flag is set. (Delay number is reached)

}

/*
                       | lcdInit function |

        Brief: The initialization sequence for the LCD.

        parameters: N/A

        return: N/A

*/

void lcdInit()

{

    P4->DIR  = (EN + RS + DATA);                    //Make pins outputs.          (Just change P4 for any other port keeping lower pins for data)
    P4->OUT &=~(BIT4 | BIT5)   ;                    //Clear the EN and RS pins.   (Just change P4 for any other port keeping BIT4 and BIT5 for E and RS)

    P4->OUT &=~ BIT5;     //Set RS to zero to write com.
    delay_ms(1);

    P4->OUT = 0x03;       //Start LCD.
    lcdTriggerEN();
    delay_ms(5);

    P4->OUT = 0x03;       //Set one.
    lcdTriggerEN();
    delay_ms(5);

    P4->OUT = 0x03;       //Set two.
    lcdTriggerEN();
    delay_ms(5);

    P4->OUT = 0x03;       //Set three.
    lcdTriggerEN();
    delay_ms(1);

    P4->OUT = 0x02;       //Set up 4-bit mode.
    lcdTriggerEN();
    lcdWriteCmd(0x28);  //2 is for 4-bit mode, 8 is for 2 lines, F has to be 0 for 5x8 dots.
    delay_ms(1);

    lcdWriteCmd(0x08);
    delay_ms(1);

    lcdWriteCmd(0x01);
    delay_ms(1);

    lcdWriteCmd(0x06);
    delay_ms(1);

    lcdWriteCmd(0x0E);  //Make this into 0x0E and the cursor will stay on the screen. (0x0C is no cursor)
    delay_ms(5);

}

void lcdTriggerEN()

{

    P4->OUT |=  EN;
    P4->OUT &= ~EN;

}

/*
                       | lcdWriteData function |

        Brief: Writing one byte of data by using the Lower_Nibble
               macro with the data parameter.

        parameters: unsigned char data

        return: N/A

*/

void lcdWriteData(unsigned char data)

{

    P4->OUT |= BIT5;              //Set RS to one to write Data.
    Lower_Nibble(data >> 4);    //Upper nibble.
    delay_ms(1);

    lcdTriggerEN();
    Lower_Nibble(data);         //Lower nibble.
    delay_ms(1);

    lcdTriggerEN();
    delay_ms(2);                //Delay >>> 47 us.

}

/*
                       | lcdWriteCmd function |

        Brief: lcdWriteCmd writes one byte of command by using the
               Lower_Nibble macro with the cmd parameter.

        parameters: unsigned char cmd

        return: N/A

*/

void lcdWriteCmd(unsigned char cmd)

{

    P4->OUT &= ~BIT5;             //Set RS to zero to write Command.
    Lower_Nibble(cmd >> 4);     //Upper nibble.
    delay_ms(1);

    lcdTriggerEN();
    Lower_Nibble(cmd);          //Lower nibble.
    delay_ms(1);

    lcdTriggerEN();
    delay_ms(2);                //Delay > 1.5ms.

}

void lcdSetText(char* text, int x, int y)

{

    int i;
    if (x < 16)

    {
        x |= 0x80;      //Set LCD for first line write.

        switch (y)

        {

        case 0:
            x |= 0x00;  //Set LCD for first line write.
            break;

        case 1:
            x |= 0x40;  //Set LCD for Second line write.
            break;

        case 2:
            x |= 0x10;  //Set LCD for Third line write.
            break;

        case 3:
            x |= 0x50;  //Set LCD for Fourth line write.
            break;

        case 5:
            x |= 0x20;  //Set LCD for second line write reverse.
            break;

        }

        lcdWriteCmd(x);

    }

    i = 0;

    while (text[i] != '\0')

    {

        lcdWriteData(text[i]);
        i++;

    }

}

void lcdSetInt(int val, int x, int y)

{

    char number_string[16];
    sprintf(number_string, "%d", val);      //Convert the integer to character string.
    lcdSetText(number_string, x, y);

}

void lcdClear()

{

    lcdWriteCmd(CLEAR);
    delay_ms(100);

}

void Door_Menu()

{

    lcdSetText("   Door  Menu    ",0,0);     //Print some FUN stuff.
    lcdSetText("1: Open Door     ",0,1);
    lcdSetText("2: Close Door    ",0,2);
    lcdSetText("3: Main Menu     ",0,3);

}

void Motor_Menu()

{

    lcdSetText("   Motor Menu    ",0,0);     //Print some FUN stuff.
    lcdSetText(" Please   Enter  ",0,1);
    lcdSetText(" Speed  Between  ",0,2);
    lcdSetText("      0 - 9      ",0,3);

}

void Lights_Menu()

{

    lcdSetText("  Lights Menu    ",0,0);     //Print some FUN stuff.
    lcdSetText("1: Red           ",0,1);
    lcdSetText("2: Green         ",0,2);
    lcdSetText("3: Blue          ",0,3);

}

void Main_Menu()

{

    lcdSetText("   Main  Menu   ",0,0);     //Print some FUN stuff.
    lcdSetText("1: Door         ",0,1);
    lcdSetText("2: Motor        ",0,2);
    lcdSetText("3: Lights       ",0,3);

}
