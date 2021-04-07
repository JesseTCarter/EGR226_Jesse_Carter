/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/17/2021

Project:        LAB07_Part1

File:           mainPart1.c

Professor:      Trevor Ekin

Description:    This program will initialize the LCD screen
                and show a blinking cursor on the first line.

*/

#include "msp.h"

void LCD_init(void);        //Prototype function for LCD initialization.
void delay_ms(int n);       //Prototype function for delays in milliseconds.
void pushNibble(unsigned char data, unsigned char control);     //Prototype function that pushes 1 nibble onto the data pins and pulses the enable pin.
void commandWrite(unsigned char command);       //Prototype function that writes one byte of command by calling the pushNibble function.
void dataWrite(unsigned char data);     //Prototype function to write one byte of data by calling the pushNibble function.

#define RS 1        //P4.0 mask.
#define RW 2        //P4.1 mask.
#define EN 4        //P4.2 mask.

/*
                       | main function |

        Brief: Main calls the LCD_init function that initializes the
               LCD display and then calls the commandWrite function
               to clear the LCD screen, then a short delay is called
               and using the commandWrite function again, the cursor
               is turned on and blinking.

        parameters: N/A

        return: N/A

*/

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    LCD_init();

    commandWrite(1);        //Clear display.

    delay_ms(500);

    while(1)

    {

        commandWrite(0x0F);

    }

}

/*
                       | pushNibble function |

        Brief: pushNibble clears the lower and upper nibbles, then
               pushes 1 nibble onto the data pins and pulses the
               enable pin.

        parameters: unsigned char data, unsigned char control

        return: N/A

*/

void pushNibble(unsigned char data, unsigned char control)

{

    data &= 0xF0;       //Clearing lower nibble.

    control &= 0x0F;        //Clearing upper nibble.

    P4OUT = data | control;     //RS = 0; R/W = 0.

    P4OUT = data | control | EN;        //Pulsing E.

    delay_ms(0);

    P4OUT = data;       //Clearing E.

    P4OUT = 0;

}

/*
                       | delay_ms function |

        Brief: delay_ms runs the delay in milliseconds when
               system clock is at 3MHz.

        parameters: int n

        return: N/A

*/

void delay_ms(int n)

{

    int i, j;

    for(j=0; j < n; j++)

        for(i = 250; i > 0; i--);       //Delay.

}

/*
                       | commandWrite function |

        Brief: commandWrite writes one byte of command by calling
               the pushNibble function with the command parameter.

        parameters: unsigned char command

        return: N/A

*/

void commandWrite(unsigned char command)

{

    pushNibble(command & 0xF0, 0);      //Upper nibble first.

    pushNibble(command << 4, 0);        //Lower nibble next.

    if (command < 4)

        delay_ms(4);        //Commands 1 and 2 need up to 1.64 milliseconds.

    else

        delay_ms(1);        //All others 40 microseconds.

}

/*
                       | dataWrite function |

        Brief: Writing one byte of data by calling the pushNibble
               function with the data parameter.

        parameters: unsigned char data

        return: N/A

*/

void dataWrite(unsigned char data)

{

    pushNibble(data & 0xF0, RS);    //Upper nibble first.

    pushNibble(data << 4, RS);      //Lower nibble next.

    delay_ms(1);

}

/*
                       | LCD_init function |

        Brief: The initialization sequence for the LCD.

        parameters: N/A

        return: N/A

*/

void LCD_init(void)

{

    P4->DIR = 0xFF;     //Makes P4 pins output for data and controls.

    delay_ms(30);       //Initialization sequence.

    pushNibble(0x30, 0);

    delay_ms(10);

    pushNibble(0x30, 0);

    delay_ms(1);

    pushNibble(0x30, 0);

    delay_ms(1);

    pushNibble(0x20, 0);        //Using 4-bit data mode.

    delay_ms(1);

    commandWrite(0x28);         //Set 4-bit data, 2-line, 5x7 font.
    commandWrite(0x06);         //Move cursor right after each char.
    commandWrite(0x01);         //Clear screen, move cursor to home.
    commandWrite(0x0F);         //Turn on display, cursor blinking.

}
