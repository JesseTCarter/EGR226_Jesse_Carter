/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           03/17/2021

Project:        LAB08_Part1

File:           mainPart1.c

Professor:      Trevor Ekin

Description:    This program controls the DC motor using the SysTick Timer. The program
                uses the SysTick timer to generate the PWM mode to create an analog signal
                for controlling the speed of the motor.

*/

#include "msp.h"

void SysTick_Init (void);
void delay_ms(uint16_t delay);
void pin_init(void);

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    SysTick_Init();

    pin_init();

    int i = 3000;

    while(1)

    {

        P2OUT |= BIT4;     //Turns the motor on.

        delay_ms(i);


    }

}

void SysTick_Init(void)

{

    SysTick-> CTRL = 0;     //Disable SysTick during step.

    SysTick-> LOAD = 0x00FFFFFF;        //Max count value.

    SysTick-> VAL = 0;      //Any write to current clears it.

    SysTick-> CTRL = 0x00000005;        //Enable SysTick, 3MHz, no interrupts

}

void delay_ms(uint16_t delay)

{

    SysTick-> LOAD = ((delay*3000)-1);      //Delay of 1ms per delay value.

    SysTick-> VAL = 0;      //Any write to CVR clears it.

    while((SysTick-> CTRL & 0x00010000) == 0);      //Wait for flag to be SET.

}

void pin_init(void)

{

    P2->SEL1 &= ~BIT4;      //Configure P2.4 as input/output.

    P2->SEL0 &= ~BIT4;

    P2->DIR |= BIT4;

    P2->OUT &= ~BIT4;       //Set P2.4 as an output pin.

}
