/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/24/2021

Project:        LAB09_Part2

File:           mainPart2.c

Professor:      Trevor Ekin

Description:    For this program, the MCU is interfaced with a 7-segment LED display
                Each segment is labeled with a letter A-G, and DP for a decimal point.
                it uses the SysTick timer interrupts in which the 7-segment LED displays the
                numbers 0-9 at a one second interval for each number.

*/

#include "msp.h"

void LED_init(void);        //Prototype function for 7-segment LED initialization.
void SysTick_init(void);        //Prototype function for SysTick initialization.
void SysTick_Handler(void);     //Prototype function for SysTick flag handler.
void Decimal_counter(void);     //Prototype function for sending a number to the 7-segment LED display.

int count;

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    LED_init();     //LED_init function being called.

    SysTick_init();     //Switch_init function being called.

    __enable_irq();     //Enables IRQ interrupts.

    while(1)

    {

        Decimal_counter();      //Decimal_counter function being called.

    }

}

/*
                       | LED_init function |

        Brief: The LED_init function initializes the proper port
               and pins for the 7-segment LED display to function.

        parameters: N/A

        return: N/A

*/

void LED_init(void)

{

    P4->SEL1 &= ~0xFF;

    P4->SEL0 &= ~0xFF;      //Configuring the GPIO's for the 7-segment LED display.

    P4->DIR |= 0xFF;        //Enabling pull resistors on all rows.

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

    SysTick-> CTRL = 0;     //Disable SysTick during step.

    SysTick-> LOAD = 3000000;       //Reload value for 1 second interrupts.

    SysTick-> VAL = 0;      //Any write to CVR clears it.

    SysTick-> CTRL = 0x00000007;        //Enable SysTick, 3MHz, with interrupts.

}

/*
                       | SysTick_Handler function |

        Brief: The SysTick_Handler function sets a timeout
               flag for SysTick, rests in main.

        parameters: N/A

        return: N/A

*/

void SysTick_Handler(void)

{

    count++;

    if(count == 10)

    {

        count = 0;

    }

}

/*
                       | Decimal_counter function |

        Brief: The Decimal_counter function gets the decimal number
               and toggles the number for what the number count is on
               to the 7-segment LED display.

        parameters: N/A

        return: N/A

*/

void Decimal_counter(void)

{

    switch(count)

    {

    case 0:

        P4->OUT = 0x3F;     //Set number 0.

        break;

    case 1:

        P4->OUT = 0x06;     //Set number 1.

        break;

    case 2:

        P4->OUT = 0x5B;     //Set number 2.

        break;

    case 3:

        P4->OUT = 0x4F;     //Set number 3.

        break;

    case 4:

        P4->OUT = 0x66;     //Set number 4.

        break;

    case 5:

        P4->OUT = 0x6D;     //Set number 5.

        break;

    case 6:

        P4->OUT = 0x7D;     //Set number 6.

        break;

    case 7:

        P4->OUT = 0x07;     //Set number 7.

        break;

    case 8:

        P4->OUT = 0x7F;     //Set number 8.

        break;

    case 9:

        P4->OUT = 0x6F;     //Set number 9.

        break;

    }

}
