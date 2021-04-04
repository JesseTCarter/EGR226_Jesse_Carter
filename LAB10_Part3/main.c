/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/04/2021

Project:        LAB10_Part3

File:           mainPart3.c

Professor:      Trevor Ekin

Description:    In this part, the code from Lab10_Part1 was modified so that it is now
                connected to a temperature sensor output voltage which displays the
                current temperature on the LCD

*/

#include "msp.h"
#include <stdio.h>

void LCD_init(void);        //Prototype function for LCD initialization.
void SysTick_delay(uint16_t delay);       //Prototype function for delays in milliseconds using the internal SysTick timer.
void pushNibble(unsigned char data, unsigned char control);     //Prototype function that pushes 1 nibble onto the data pins and pulses the enable pin.
void commandWrite(unsigned char command);       //Prototype function that writes one byte of command by calling the pushNibble function.
void dataWrite(unsigned char data);     //Prototype function to write one byte of data by calling the pushNibble function.
void ADC_init(void);        //Prototype function that initializes analog input channel A0 using port 5 pin 5, which is configured for Analog-to-digital conversion.
void SysTick_init(void);        //Prototype function for SysTick initialization.
void delay_ms(int n);       //Prototype function for delays in milliseconds by counting down from an integer.
void LCD_print(void);       //Prototype function for printing characters to be displayed on the LCD.

#define RS 1
#define RW 2
#define EN 4

float volt;
float temp;

uint16_t result;

void main(void)

{

    SysTick_init();

    ADC_init();

    LCD_init();

    while(1)

    {
        commandWrite(1);        //Clears the display.

        ADC14 -> CTL0 |= 1;       //Starts a conversion.

        while(!ADC14 -> IFGR0);       //Will wait until conversion is completed.

        result = ADC14 -> MEM[0];     //Immediately stores the value into a variable.

        volt = (result * 3.3) / 16384;

        temp = (volt - 0.5) / 0.01;

        LCD_print();

        printf("Current Temp. is:\n\t%.1f\n", temp);

        SysTick_delay(1000);

    }

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

        for(i = 250; i > 0; i--);       //Delay

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
                       | ADC_init function |

        Brief: ADC_init will initialize the analog input channel A0 using
               port 5 pin 5 which is set to use analog-to-digital conversion
               (ADC). ADC is in 14-bit resolution with the 1/4 clock divider,
               S/H pulse mode, SW trigger enabled, MCLCK, and 32 sample clock.

        parameters: N/A

        return: N/A

*/

void ADC_init(void)

{

    ADC14 -> CTL0 = 0x00000010;     //Powers on and is disabled during configuration.

    ADC14 -> CTL0 |= 0x04D80300;        //S/H pulse mode, MCLCK, 32 Sample clocks, SW trigger, 1/4 clock divider.

    ADC14 -> CTL1 = 0x00000030;     //14-bit resolution mode.

    ADC14 -> MCTL[0] = 0;       //A0 input, single ended, vref = avcc.

    P5 -> SEL1 |= 0x20;     //Configures port 5 pin 5 for A0.

    P5 -> SEL0 |= 0x20;

    ADC14 -> CTL1 |= 0x0000000;     //Starts conversion at memory register 0.

    ADC14 -> CTL0 |= 2;     //Enables ADC after configuration is done.

}

/*
                       | SysTick_init function |

        Brief: The SysTick_init function initializes the MSP432's
               built-in internal timer for more accurate delays.

        parameters: N/A

        return: N/A

*/

void SysTick_init(void)

{

    SysTick -> CTRL = 0;     //Disable SysTick during step.

    SysTick -> LOAD = 0x00FFFFFF;        //Max reload value.

    SysTick -> VAL = 0;      //Any write to CVR clears it.

    SysTick -> CTRL = 0x00000005;        //Enable SysTick, 3MHz, no interrupts.

}

/*
                       | SysTick_delay function |

        Brief: SysTick_delay runs the delay in milliseconds when
               system clock is at 3MHz.

        parameters: uint16_t delay

        return: N/A

*/

void SysTick_delay(uint16_t delay)

{

    SysTick -> LOAD = ((delay * 3000) - 1);      //Delay of 1ms per delay value.

    SysTick -> VAL = 0;      //Any write to CVR clears it.

    while((SysTick -> CTRL & 0x00010000) == 0);      //Wait for flag to be SET.

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

/*
                       | LCD_print function |

        Brief: The LCD_print function, stores a 17 char string into a
               2D array and calls dataWrite to print the string from
               DDRAM onto the LCD, then the commandWrite function is
               called to print the string onto the proper line for
               the LCD display output.

        parameters: N/A

        return: N/A

*/

void LCD_print(void)

{

    uint8_t i;

    char tempis[17] =   "Current Temp. is:";
    char number[17];

    sprintf(number, "%.1f      ", temp);

    for(i = 0; i < 16; i++)

    {

        dataWrite(tempis[i]);

        delay_ms(10);

    }

    commandWrite(0xC0);         //Move to line 2 (address 0x40) -> 0b11000000.

    for(i = 0; i < 9; i++)

    {

        dataWrite(number[i]);

        delay_ms(10);

    }

}
