/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/03/2021

Project:        LAB06_Part2

File:           mainPart2.c

Professor:      Trevor Ekin

Description:    This program will collect a key value and store it in a variable
                corresponding to the key that is pressed on the external keypad
                that is wired to the MSP432. Once four valid values have been input
                by the user, the four stored values will be shown after the user
                presses the pound key.

*/

#include "msp.h"
#include <stdio.h>

void Keypad_init(void);        //Prototype function for keypad initialization.
uint8_t Keypad_read(void);      //Prototype function for reading the key inputs.
void Print_keys(void);      //Prototype function for print statements for key presses.
void Read_passcodeinput(void);     //Prototype function for reading and storing password pin user input values.
void Reset_passcode(void);      //Prototype function for resetting the password pin and start over.
void SysTick_init (void);        //Prototype function for SysTick initialization.
void SysTick_delay(uint16_t delay);     //Prototype function for setting up SysTick delay value.


uint8_t count, press;
uint8_t Passcode1 = 20;
uint8_t Passcode2 = 20;
uint8_t Passcode3 = 20;
uint8_t Passcode4 = 20;
uint8_t Passcode5 = 20;

/*
                       | main function |

        Brief: Main calls the SysTick_init and Keypad_init functions
               to initialize the internal SysTick timer, and the ports
               and pins that interface with the external keypad. The user
               is prompted to enter a four digit pin code on the keypad and
               then press #. Within the while loop the function Keypad_read is
               called and determines which keys have been pressed, those
               values are stored, returned, and then printed once the function
               Print_keys and Read_passcodeinput have been called. A delay
               of 10 milliseconds is called, and the whole process starts again.

        parameters: N/A

        return: N/A

*/

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    SysTick_init();     //Calls the function that initializes SysTick.

    Keypad_init();      //Calls the function that initializes the pins for the keypad.

    printf("Enter a 4 digit numerical PIN code followed by #: \n");

    while(1)

    {

        press=Keypad_read();      //Calls the function that will read the keypad.

        if(press)

        {

            Print_keys();

        }

        Read_passcodeinput();

        if(Passcode1 !=20 && Passcode5 == 12)

        {

            printf("Your 4 digit numerical PIN code is: ");

            printf("%d %d %d %d \n", Passcode1, Passcode2, Passcode3, Passcode4);

            Reset_passcode();

            printf("Enter a 4 digit numerical PIN code followed by #: \n");

        }

        SysTick_delay(10);      //SysTick delay for 10 seconds.

    }

}

/*
                       | Keypad_init function |

        Brief: The keypad_init function initializes the required
               ports and pins on the MSP432 for the program to
               function correctly.

        parameters: N/A

        return: N/A

*/

void Keypad_init(void)

{
    P4->SEL1 &= ~0xFF;      //Setting the keypad rows 0, 1, 2, and 3, to P4.0, P4.1, P4.2, and P4.3.
    P4->SEL0 &= ~0xFF;

    P4->DIR &= ~(BIT0| BIT1 | BIT2 | BIT3);      //Pull resistor enabled for rows 0, 1, 2, and 3, on pins P4.0, P4.1, P4.2, and P4.3.
    P4->REN |= (BIT0 | BIT1 | BIT2 | BIT3);      //Configuring rows with pull-up resistors, P4.0, P4.1, P4.2, and P4.3.
    P4->OUT |= (BIT0 | BIT1 | BIT2 | BIT3);      //Setting the keypad columns as inputs on P4.6, P4.5, and P4.4.
    P4->DIR &= ~(BIT4 | BIT5| BIT6);

}

/*
                       | Keypad_read function |

        Brief: The Keypad_read function sets the columns on the keypad as
               inputs, but column 3 is set to low, it reads the rows on
               the keypad and if one of the rows come back as low, a key
               is pressed, if the corresponding row value that was pressed
               matches one of the if statements row values, the variable count
               will equal that key in the row, and returns a value of 1.

        parameters: N/A

        return: uint8_t

*/

uint8_t Keypad_read(void)

{

    uint8_t col, row;

    for(col = 0; col <3; col++)

    {

        P4->DIR = 0x00;     //Setting all columns as inputs.

        P4->DIR |= BIT(4+col);      //Setting column 3 as output.

        P4->OUT &= ~BIT(4+col);     //Setting column 3 to LOW.

        SysTick_delay(20);      //Delaying the while loop for 20 milliseconds.

        row = P4->IN & 0x0F;        //Reading all the rows on the keypad.

        while (!(P4IN & BIT0)|!(P4IN & BIT1)|!( P4IN & BIT2)|!( P4IN & BIT3));

        if(row != 0x0F)

            break;      //If one of the inputs comes back as low, a key is pressed.

    }

    P2->DIR = 0x00;     //Setting columns as inputs.

    if(col == 3)

        return 0;

    if(row == 0x0E) count = col+1;        //Key in row 0.

    if(row == 0x0D) count = 3+col+1;      //Key in row 1.

    if(row == 0x0B) count = 6+col+1;      //Key in row 2.

    if(row == 0x07) count = 9+col+1;      //Key in row 3.

        return 1;

}

/*
                       | Print_keys function |

        Brief: The Print_keys function prints out the key
               pressed that corresponds to the count value.

        parameters: N/A

        return: N/A

*/

void Print_keys(void)

{

    if (count == 1)
        printf("1\n");

    else if (count == 2)
        printf("2\n");

    else if (count == 3)
        printf("3\n");

    else if (count == 4)
        printf("4\n");

    else if (count == 5)
        printf("5\n");

    else if (count == 6)
        printf("6\n");

    else if (count == 7)
        printf("7\n");

    else if (count == 8)
        printf("8\n");

    else if (count == 9)
        printf("9\n");

    else if (count == 10)
        printf("*\n");

    else if (count == 11)

    {

        printf("0\n");

        count = 0;

    }

    else if (count == 12)
        printf("#\n");
}

/*
                       | Read_passcodeinput function |

        Brief: The Read_passcodeinput function gets the values for
               each key press, and stores that value into into a variable.

        parameters: N/A

        return: N/A

*/

void Read_passcodeinput(void)

{

    if(count == 10);

    else if (count == 12 && Passcode1 == 20);

        else if (count == 12 && Passcode1 != 20) Passcode5 = count;

            else if (press == 0);

            else

            {

                Passcode1 = Passcode2;
                Passcode2 = Passcode3;
                Passcode3 = Passcode4;
                Passcode4 = count;

            }

}

/*
                       | Reset_passcode function |

        Brief: The Reset_passcode function resets the passcode
               variable values back to twenty.

        parameters: N/A

        return: N/A

*/

void Reset_passcode(void)

{

    Passcode1 = 20;
    Passcode2 = 20;
    Passcode3 = 20;
    Passcode4 = 20;
    Passcode5 = 20;

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
    SysTick-> LOAD = 0x00FFFFFF;        //Max reload value.
    SysTick-> VAL = 0;      //Any write to CVR clears it.
    SysTick-> CTRL = 0x00000005;        //Enable SysTick, 3MHz, no interrupts.

}

/*
                       | SysTick_delay function |

        Brief: The SysTick_delay function sets up the SysTick
               timer delay value, it's setup for 1ms delays
               per 1 value of SysTick_delay.

        parameters: uint16_t delay

        return: N/A

*/

void SysTick_delay(uint16_t delay)

{

    SysTick-> LOAD = ((delay*3000)-1);      //Delay for 1ms per delay value.
    SysTick-> VAL = 0;      //Any write to CVR clears it.
    while((SysTick-> CTRL & 0x00010000) == 0);      //Wait for flag to be SET.

}
