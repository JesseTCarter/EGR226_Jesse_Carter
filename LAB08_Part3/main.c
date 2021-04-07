/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/17/2021

Project:        LAB08_Part3

File:           mainPart3.c

Professor:      Trevor Ekin

Description:    This program controls the DC motor using Timer_A#, SysTick,
                and an external keypad. The program uses Timer_A# to generate
                the PWM mode to create an analog signal for controlling the
                speed of the motor. SysTick generates the delays, and the keypad
                allows the user to enter a percentage to increase or decrease
                the duty cycle of the motor, respectively changing the speed.

*/

#include "msp.h"
#include <stdio.h>
#include <math.h>

void Pin_init(void);        //Prototype function for pin initialization.
void TimerA_init(void);     //Prototype function for Timer_A# initialization.
void SysTick_init(void);        //Prototype function for SysTick initialization.
void SysTick_delay(uint16_t delay);       //Prototype function for delays in milliseconds.
void Keypad_init(void);        //Prototype function for keypad initialization.
void Motor_speed(int i, int duty[]);      //Prototype function for motor speed calculations.
uint8_t Keypad_read(void);      //Prototype function for reading the key inputs.

uint8_t num;

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    Keypad_init();      //Keypad_init function being called.

    SysTick_init();     //SysTick_init function being called.

    Pin_init();     //Pin_init function being called.

    TimerA_init();      //TimerA_init function being called.

    int Duty_cycle[5];

    int i = 0;

    printf("Enter a duty cycle percentage on the keypad followed by the # key\n");

    while(1)

    {

        if(Keypad_read())       //Calls the function that will read the keypad.

        {

            i++;

            if(num < 10)    Duty_cycle[i] = num;

            if(num == 11)   Duty_cycle[i] = 0;

            if(num == 10)

            {

                printf("Error\n");

                printf("Enter a valid duty cycle percentage.\n");

                i = 0;

            }

            if(num == 12)       //When the # key is pressed on the keypad.

            {

                Motor_speed(i, Duty_cycle);

                i = 0;

            }

        }

    }

}

/*
                       | Motor_speed function |

        Brief: The function Motor_speed uses integer i and a small duty
               array of 5 to calculate the duty cycle of the DC motor
               and takes that value from keypad presses and sets it to
               Timer_A#'s CCR[1] value, which is the value that controls
               the overall speed of the motor using the period it takes for
               its signal to complete an on-and-off cycle.

        parameters: int i, int duty[]

        return: N/A

*/

void Motor_speed(int i, int duty[])

{

    int value;

    int Cycle_duty;

    if(i == 3)

    {

        value = ((duty[1] * 10) + (duty[2] * 1));

        Cycle_duty = ((value * TIMER_A0->CCR[0]) / 100);

        TIMER_A0->CCR[1] = Cycle_duty;

    }

    if(i == 4)

    {

        value = ((duty[1] * 100) + (duty[2] * 10) + (duty[3] * 1));

        Cycle_duty = ((value * TIMER_A0->CCR[0]) / 100);

        TIMER_A0->CCR[1] = Cycle_duty;

    }

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

    uint8_t col;

    uint8_t row;

    for(col = 0; col < 3; col++)

    {

        P4->DIR = 0;        //Setting all columns as inputs.

        P4->DIR |=  BIT(4 + col);       //Setting columns 0 - 2 to output.

        P4->OUT &=~ BIT(4 + col);       //Setting columns 0 - 2 to LOW.

        SysTick_delay(10);      //Delaying the while loop for 10 milliseconds.

        row = P4->IN & 0x0F;        //Reading all the rows on the keypad.


        while(!(P4->IN & BIT0) | !(P4->IN & BIT1) | !(P4->IN & BIT2) | !(P4->IN & BIT3));

        if(row != 0x0F) break;      //If one of the inputs comes back as low, a key is pressed.

    }

    P4->DIR = 0;        //Setting columns as inputs.

    if(col == 3)        //Fail-safe because col only goes to 2.

        return 0;


    if(row == 0x0E)     num = col + 1;      //Key in row 0.

    if(row == 0x0D)     num = 3 + col + 1;      //Key in row 1.

    if(row == 0x0B)     num = 6 + col + 1;      //Key in row 2.

    if(row == 0x07)     num = 9 + col + 1;      //Key in row 3.

    return 1;

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

    P4->SEL1 &= ~0xFF;

    P4->SEL0 &= ~0xFF;      //Setting the keypad rows 0, 1, 2, and 3, to P4.0, P4.1, P4.2, and P4.3.

    P4->DIR &= ~(BIT0| BIT1 | BIT2 | BIT3);      //Pull resistor enabled for rows 0, 1, 2, and 3, on pins P4.0, P4.1, P4.2, and P4.3.

    P4->REN |= (BIT0 | BIT1 | BIT2 | BIT3);      //Configuring rows with pull-up resistors, P4.0, P4.1, P4.2, and P4.3.

    P4->OUT |= (BIT0 | BIT1 | BIT2 | BIT3);      //Setting the keypad columns as inputs on P4.6, P4.5, and P4.4.

    P4->DIR &= ~(BIT4 | BIT5| BIT6);

}

/*
                       | Pin_init function |

        Brief: The Pin_inst function initializes the required
               ports and pins on the MSP432 for the program.

        parameters: N/A

        return: N/A

*/

void Pin_init(void)

{

    P2->SEL1 &= ~BIT4;      //Configuring port 2 pin 4 as GPIO for pulse-width modulation.

    P2->SEL0 |= BIT4;       //Port 2 pin 4 set to TimerA_0.1.

    P2->DIR |= BIT4;

}

/*
                       | TimerA_init function |

        Brief: This function initializes the Timer_A#
               peripherals to generate a PWM on a DC motor.

        parameters: N/A

        return: N/A
*/

void TimerA_init(void)

{

    TIMER_A0->CCR[0] = 1000 - 1;

    TIMER_A0->CCTL[1] = 0x0070;

    TIMER_A0->CCR[1] = 1000;        //Initializes motor to be off using duty cycle.

    TIMER_A0->CTL = 0x0214;

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
