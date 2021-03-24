/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/24/2021

Project:        LAB09_Part3

File:           mainPart3.c

Professor:      Trevor Ekin

Description:    For this program, the MCU is interfaced with a 7-segment LED display
                Each segment is labeled with a letter A-G, and DP for a decimal point.
                the program uses port interrupts for 2 switch push buttons, button 1 is
                for incrementing the 7-segment LED display and button 2 is for decrementing
                the 7-segment LED display.

*/

#include "msp.h"

void LED_init(void);        //Prototype function for 7-segment LED initialization.
void Decimal_counter(void);     //Prototype function for sending a number to the 7-segment LED display.
void Switch_init(void);     //Prototype function for the 3 push buttons initialization.
void PORT2_IRGHandler(void);        //Prototype function for detecting an interrupt and resetting interrupt flag.

int num;

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    LED_init();     //LED_init function being called.

    Switch_init();      //Switch_init function being called.

    NVIC_EnableIRQ(PORT2_IRQn);     //Enables PORT2 interrupts.

    __enable_interrupt();       //Enables global interrupts.

    while(1)

    {

        ;       //Hardware does the work, so nothing needs to be inside the while loop.

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
                       | Decimal_counter function |

        Brief: The Decimal_counter function gets the decimal number
               and toggles the number for what the number count is on
               to the 7-segment LED display.

        parameters: N/A

        return: N/A

*/

void Decimal_counter(void)

{

    switch(num)

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

/*
                       | Switch_init function |

        Brief: The Switch_init function initializes the proper ports and pins
               for the 2 external push button switches that have pull-up resistors
               enabled. Pressing button 1 or 2 will trigger the ISR interrupt and
               will toggle port 4 where the 7-segment LED display is connected.

        parameters: N/A

        return: N/A
*/

void Switch_init(void)

{

    P2->SEL0 &= ~(BIT5 | BIT6);     //Setting port 2, pins 5, and 6, as GPIO as input/output.

    P2->SEL1 &= ~(BIT5 | BIT6);

    P2->DIR &= ~(BIT5 | BIT6);

    P2->REN |= (BIT5 | BIT6);       //Enabling internal resistor.

    P2->OUT |= (BIT5 | BIT6);       //Pull-up resistor enabled on port 2, pins 5, and 6; output set to high.

    P2->IES |= (BIT5 | BIT6);       //Setting pin interrupts to trigger when pin goes from high to low.

    P2->IE  |= (BIT5 | BIT6);       //Enabling interrupt for port 2, pins 5, and 6.

    P2->IFG = 0;        //Clearing all port 2 interrupt flags.

}

/*
                       | PORT2_IRGHandler function |

        Brief: The PORT2_IRGHandler function detects if port 2, pins 5, and 6
               had an interrupt if it did, the function will toggle the output
               on the 7-segment LED display by calling the Decimal_counter function.
               The program will then reset the interrupt flag on the push-button switch.

        parameters: N/A

        return: N/A
*/

void PORT2_IRQHandler(void)

{

    if(P2->IFG & BIT5)      //If port 2, pin 5 has an interrupt.

    {

        num++;      //Increase num by one interval.

        if(num == 10)

        {

            num = 0;

        }

        Decimal_counter();      //Calling the Decimal_counter function to toggle the output on the 7-segment LED display.

    }

    if(P2->IFG & BIT6)      //If port 2, pin 6 has an interrupt.

    {

        num--;      //Decrease num by one interval.

        if(num == -1)

        {

            num = 9;

        }

        Decimal_counter();      //Calling the Decimal_counter function to toggle the output on the 7-segment LED display.

    }

    P2->IFG &= ~(BIT5 | BIT6);      //Resetting the interrupt flag.

}
