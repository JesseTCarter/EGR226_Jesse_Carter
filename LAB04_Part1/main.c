/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           02/17/2021

Project:        LAB04_Part1

File:           main.c

Professor:      Trevor Ekin

Description:    A program that checks if a button is pressed, if pressed LED turns on and if pressed
                again led will cycle through three colors, red, green, blue. Incorporates a de-bounce switch
                function to solve for the switch bounce, so that the button presses are accurate with
                changing the LED colors, so that no interference accidentally changes the LED color.

*/

#include "msp.h"

void pin_inst(void);        //Prototype function for pin initialization.
int DebounceSwitch1(void);      //Prototype function for checking for De-bounce.
void delay(int);        //Prototype function for to pass a delay time in milliseconds.

void main(void)

    {

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    pin_inst();

    int color = 0;

    while(1)

    {

        if(DebounceSwitch1())

        {

            P2OUT ^= BIT(color);
                if(color == 3) color = 0;
            P2OUT = BIT(color);
            color++;

        }

    }

}

/*
                       | pin_inst function |

        Brief: The pin_inst function initializes the required
               ports and pins on the MSP432.

        parameters: N/A

        return: N/A

*/

void pin_inst(void)

{

    P1->SEL1 &= ~BIT1;      //Configure P1.1 as input/output.
    P1->SEL0 &= ~BIT1;

    P1->DIR &= ~BIT1;       //P1.1 set as input.

    P1->REN |= BIT1;        //P1.1 pull resistor enabled.

    P1->OUT |= BIT1;        //Pull up/down is selected by P1 -> OUT.

    P2->SEL1 &= ~BIT0;      //Configure P2.0 as input/output.
    P2->SEL0 &= ~BIT0;

    P2->DIR |= BIT0;        //P2.0 set as output pin.

    P2->SEL1 &= ~BIT1;      //Configure P2.1 as input/output.
    P2->SEL0 &= ~BIT1;

    P2->DIR |= BIT1;        //P2.1 set as output pin.

    P2->SEL1 &= ~BIT2;      //Configure P2.2 as input/output.
    P2->SEL0 &= ~BIT2;

    P2->DIR |= BIT2;        //P2.2 set as output pin.

    P2->OUT &= BIT0;
    P2->OUT &= BIT1;
    P2->OUT &= BIT2;

}

/*
                       | delay function |

        Brief: The delay function passes millisecond delays
               as a function argument

        parameters: int time

        return: N/A

*/

void delay(int time)        //Delay function to pass millisecond delays as a function argument.

{

    int num = time;
    int i = 0;

    for(i = 0; i < num; i++)

    {

        __delay_cycles(7500);

    }

}

/*
                       | DebounceSwitch1 function |

        Brief: The DebounceSwitch1 function sets a variable called
               "pin_Value" to 0, and a variable called "time" to 75.
               An if loop checks if an on-board button is pressed, if
               the button is pressed, the on-board LED's will cycle through
               the colors red, green, and blue.

        parameters: N/A

        return: int

*/

int DebounceSwitch1(void)

{

    int pin_Value = 0;      //Initialize variable as low.
    int time = 75;

    if((P1->IN & BIT1) == 0)        //Check if button is pressed.

    {

        delay(time);        //Pause for  milliseconds for switch bounce.

        if((P1->IN & BIT1) == 0)        //If button is not pressed, variable is low, else if the button is pressed, variable high.

        {

            pin_Value = 0;
        }

        else

        {
            pin_Value = 1;
        }

    }

    return pin_Value;       //Return 1 if pressed, 0 if not pressed.

}
