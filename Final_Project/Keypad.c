/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/24/2021

Project:        Keypad Source File

File:           Keypad.c

Description:    This is a source file for the integrated keypad
                that contains its C function code, macro
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

#include <Keypad.h>
#include <stdint.h>
#include <stdio.h>
#include "LCD.h"
#include "Dc_motor.h"
#include "Servo_motor.h"
#include "Timer_A_lights.h"
#include "ISR.h"


uint8_t num;
uint8_t temp;

int j = 1;

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

    P2->SEL1 &= ~0x78;
    P2->SEL0 &= ~0x78;                                        //Set keypad Row 0 to P4.0 Row 1 to P4.1 and Row2 to P4.2 and Row3 to P4.3.

    P2->DIR &=~(BIT3| BIT4 | BIT5 | BIT6);                    //Enable pull resistor on all rows P2.4, P2.5, P2.6, P2.7.
    P2->REN |=(BIT3| BIT4 | BIT5 | BIT6);                     //Configure rows with pull-up rows P2.4, P2.5, P2.6, P2.7.
    P2->OUT |=(BIT3| BIT4 | BIT5 | BIT6);

    P3->SEL1 &= ~0x70;
    P3->SEL0 &= ~0x70;
    P3->DIR &=~(BIT5 | BIT6| BIT7);                           //Set keypad columns to inputs P3.5, P3.6, P3.7.

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

        P3->DIR = 0x00;                                        //Set Columns to inputs.
        P3->DIR |= BIT(5+col);                                 //Set Column  3 to output.
        P3->OUT &= ~BIT(5+col);                                //Set Column 3 to low.

        delay_ms(20);                                          //Delay the while loop.
        row = P2->IN & 0x78;                                   //Read all rows.

        while (!(P2IN & BIT3)|!(P2IN & BIT4)|!( P2IN & BIT5)|!( P2IN & BIT6));

        if(row != 0x78) break;                                 //If one of the input is low, some key is pressed.

    }

    if(col == 3)

        return 0;

    if(row == 0x70) num = col+1;                               //Key in row 0.
    if(row == 0x68) num = 3+col+1;                             //Key in row 1.
    if(row == 0x58) num = 6+col+1;                             //Key in row 2.
    if(row == 0x38) num = 9+col+1;                             //Key in row 3.

    return 1;

}

/*

                       | Print_Main_Menu function |

        Brief: The Print_Main_Menu function is a short state machine,
               it gets called within another function, and whatever
               the variable num is equal to, case 1, 2, or 3 is initiated
               and calls another function.

        parameters: N/A

        return: N/A

*/

void Print_Main_Menu(void)

{

    switch(num)

    {

    case 1:
        Door_Menu();
        break;

    case 2:
        Motor_Menu();
        break;

    case 3:
        Lights_Menu();
        break;

    }

}

/*

                       | Print_Door_Menu function |

        Brief: The Print_Door_Menu function is a short state machine,
               it gets called within another function, and whatever
               the variable num is equal to, case 1, 2, or 3 is initiated
               and calls another function, whatever function is called,
               either the servo motor will go into an open or closed door
               position, or the user will be returned to the main menu.

        parameters: N/A

        return: N/A

*/

void Print_Door_Menu(void)

{
    j = 1;

    while(j == 1)

    {

        pressed=Keypad_read();

        if(pressed)

        {

            j = 0;

        }

    }

    switch(num)

    {

    case 1:
        TIMER_A2->CCR[1] = 1600;        //Open servo door.
        P2->OUT &= ~BIT0;
        P2->OUT |= BIT1;
        Menu_Loop();
        break;

    case 2:
        TIMER_A2->CCR[1] = 4500;        //Close servo door.
        P2->OUT &= ~BIT1;
        P2->OUT |= BIT0;
        Menu_Loop();
        break;

    case 3:
        Menu_Loop();
        break;

    }

}

/*

                       | Print_Lights_Menu function |

        Brief: The Print_Lights_Menu function is a short state machine,
               it gets called within another function, and whatever
               the variable num is equal to, case 1, 2, or 3 is initiated
               and calls another function, whatever function is called,
               the user is sent to either the red, green, or blue RGB
               menu, and the user can press a number 0-9 to change the
               LEDS brightness.

        parameters: N/A

        return: N/A

*/

void Print_Lights_Menu(void)

{

    j = 1;

    while(j == 1)

    {

        pressed=Keypad_read();

        if(pressed)

        {

            j = 0;

        }

    }

    switch(num)

    {

    case 1:
        lcdSetText("    Red Light    ",0,0);
        lcdSetText("     Set  the    ",0,1);
        lcdSetText(" Brightness from ",0,2);
        lcdSetText("     0 - 100     ",0,3);
        pressed=Keypad_read();
        Red_brightness();
        break;

    case 2:
        lcdSetText("  Green Light    ",0,0);
        lcdSetText("     Set  the    ",0,1);
        lcdSetText(" Brightness from ",0,2);
        lcdSetText("     0 - 100     ",0,3);
        pressed=Keypad_read();
        Green_brightness();
        break;

    case 3:
        lcdSetText("   Blue Light    ",0,0);
        lcdSetText("     Set  the    ",0,1);
        lcdSetText(" Brightness from ",0,2);
        lcdSetText("     0 - 100     ",0,3);
        pressed=Keypad_read();
        Blue_brightness();
        break;

    }

}

/*

                       | Get_Motor_Speed function |

        Brief: The Get_Motor_Speed function is a short state machine,
               it gets called within another function, and whatever
               the variable num is equal to, case 1-9 is initiated
               and changes the DC motor speed, and returns the user
               back to the main menu.

        parameters: N/A

        return: N/A

*/

void Get_Motor_Speed(void)

{

    j = 1;

    while(j == 1)

    {

        pressed=Keypad_read();

        if(pressed)

        {

            j = 0;

        }

    }

    switch(num)

    {

    case 11:
        TIMER_A0->CCR[4] = 0;
        Menu_Loop();
        break;

    case 1:
        TIMER_A0->CCR[4] = 9375*1/9;
        Menu_Loop();
        break;

    case 2:
        TIMER_A0->CCR[4] = 9375*2/9;
        Menu_Loop();
        break;

    case 3:
        TIMER_A0->CCR[4] = 9375*3/9;
        Menu_Loop();
        break;

    case 4:
        TIMER_A0->CCR[4] = 9375*4/9;
        Menu_Loop();
        break;

    case 5:
        TIMER_A0->CCR[4] = 9375*5/9;
        Menu_Loop();
        break;

    case 6:
        TIMER_A0->CCR[4] = 9375*6/9;
        Menu_Loop();
        break;

    case 7:
        TIMER_A0->CCR[4] = 9375*7/9;
        Menu_Loop();
        break;

    case 8:
        TIMER_A0->CCR[4] = 9375*8/9;
        Menu_Loop();
        break;

    case 9:
        TIMER_A0->CCR[4] = 9375;
        Menu_Loop();
        break;

    }

}

/*

                       | Red_brightness function |

        Brief: The Red_brightness function is a state machine, it
               gets called within another state machine, and whatever
               the variable num is equal to, case 1-9 is initiated
               and changes the LED brightness, and returns the user
               back to the main menu.

        parameters: N/A

        return: N/A

*/

void Red_brightness(void)

{

    j = 1;

    while(j == 1)

    {

        pressed=Keypad_read();

        if(pressed)

        {

            j = 0;

        }

    }

    switch(num)

    {

    case 11:
        TIMER_A2->CCR[3] = 0;
        Menu_Loop();
        break;

    case 1:
        TIMER_A2->CCR[3] = 60000*1/9;
        Menu_Loop();
        break;

    case 2:
        TIMER_A2->CCR[3] = 60000*2/9;
        Menu_Loop();
        break;

    case 3:
        TIMER_A2->CCR[3] = 60000*3/9;
        Menu_Loop();
        break;

    case 4:
        TIMER_A2->CCR[3] = 60000*4/9;
        Menu_Loop();
        break;

    case 5:
        TIMER_A2->CCR[3] = 60000*5/9;
        Menu_Loop();
        break;

    case 6:
        TIMER_A2->CCR[3] = 60000*6/9;
        Menu_Loop();
        break;

    case 7:
        TIMER_A2->CCR[3] = 60000*7/9;
        Menu_Loop();
        break;

    case 8:
        TIMER_A2->CCR[3] = 60000*8/9;
        Menu_Loop();
        break;

    case 9:
        TIMER_A2->CCR[3] = 60000;
        Menu_Loop();
        break;

    }

}

/*

                       | Blue_brightness function |

        Brief: The Blue_brightness function is a state machine, it
               gets called within another state machine, and whatever
               the variable num is equal to, case 1-9 is initiated
               and changes the LED brightness, and returns the user
               back to the main menu.

        parameters: N/A

        return: N/A

*/

void Blue_brightness(void)

{

    j = 1;

    while(j == 1)

    {

        pressed=Keypad_read();

        if(pressed)

        {

            j = 0;

        }

    }

    switch(num)

    {

    case 11:
        TIMER_A2->CCR[4] = 0;
        Menu_Loop();
        break;

    case 1:
        TIMER_A2->CCR[4] = 60000*1/9;
        Menu_Loop();
        break;

    case 2:
        TIMER_A2->CCR[4] = 60000*2/9;
        Menu_Loop();
        break;

    case 3:
        TIMER_A2->CCR[4] = 60000*3/9;
        Menu_Loop();
        break;

    case 4:
        TIMER_A2->CCR[4] = 60000*4/9;
        Menu_Loop();
        break;

    case 5:
        TIMER_A2->CCR[4] = 60000*5/9;
        Menu_Loop();
        break;

    case 6:
        TIMER_A2->CCR[4] = 60000*6/9;
        Menu_Loop();
        break;

    case 7:
        TIMER_A2->CCR[4] = 60000*7/9;
        Menu_Loop();
        break;

    case 8:
        TIMER_A2->CCR[4] = 60000*8/9;
        Menu_Loop();
        break;

    case 9:
        TIMER_A2->CCR[4] = 60000;
        Menu_Loop();
        break;

    }

}

/*

                       | Green_brightness function |

        Brief: The Green_brightness function is a state machine, it
               gets called within another state machine, and whatever
               the variable num is equal to, case 1-9 is initiated
               and changes the LED brightness, and returns the user
               back to the main menu.

        parameters: N/A

        return: N/A

*/

void Green_brightness(void)

{

    j = 1;

    while(j == 1)

    {

        pressed=Keypad_read();

        if(pressed)

        {

            j = 0;

        }

    }

    switch(num)

    {

    case 11:
        TIMER_A2->CCR[2] = 0;
        Menu_Loop();
        break;

    case 1:
        TIMER_A2->CCR[2] = 60000*1/9;
        Menu_Loop();
        break;

    case 2:
        TIMER_A2->CCR[2] = 60000*2/9;
        Menu_Loop();
        break;

    case 3:
        TIMER_A2->CCR[2] = 60000*3/9;
        Menu_Loop();
        break;

    case 4:
        TIMER_A2->CCR[2] = 60000*4/9;
        Menu_Loop();
        break;

    case 5:
        TIMER_A2->CCR[2] = 60000*5/9;
        Menu_Loop();
        break;

    case 6:
        TIMER_A2->CCR[2] = 60000*6/9;
        Menu_Loop();
        break;

    case 7:
        TIMER_A2->CCR[2] = 60000*7/9;
        Menu_Loop();
        break;


    case 8:
        TIMER_A2->CCR[2] = 60000*8/9;
        Menu_Loop();
        break;

    case 9:
        TIMER_A2->CCR[2] = 60000;
        Menu_Loop();
        break;

    }

}

/*

                       | Menu_Loop function |

        Brief: The Menu_Loop function is a short state machine, it
               is called within main, and within the function, it
               takes the user to the main menu by calling the main
               menu function, next, if a button is pressed, the user
               can go into one of three menus, to control the external
               devices.

        parameters: N/A

        return: N/A

*/

void Menu_Loop(void)

{

    Main_Menu();

    while (1)

    {

        pressed=Keypad_read();        //Calls the function to read the keypad.

        if(pressed)

        {

        Print_Main_Menu();
        temp = num;
        pressed=Keypad_read();

        switch(temp)

            {

            case 1:
                pressed=Keypad_read();
                Print_Door_Menu();
                break;

            case 2:
                pressed=Keypad_read();
                Get_Motor_Speed();
                break;

            case 3:
                pressed=Keypad_read();
                Print_Lights_Menu();
                break;

            }

        }

    }

}
