/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/25/2021

Project:        Final_Project

File:           main.c

Professor:      Trevor Ekin

Description:    This program utilizes everything taught through
                the laboratories of EGR 226 and combines them all
                together into one last final project. The user is
                brought to a menu displayed on an LCD, and is able
                to choose from 3 options, door, motor, and lights.
                The user can either open or close the door, change
                the speed of the motor, or operate the lights. There
                is an emergency stop button for the motor, and an off
                and on button for the lights. The user can also change
                the brightness of the display, or the contrast, using
                the external potentiometers.

*/

#include <stdio.h>
#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include "DC_motor.h"
#include "Servo_motor.h"
#include "Timer_A_lights.h"
#include "ISR.h"

int main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    SysTick_initialization();                       //Initializes the SysTick Timer.
    Keypad_init();                                  //Initializes the Keypad.
    lcdInit();                                      //Initializes the LCD.
    Timer_A_init();                                 //Initializes the Timer_A peripheral.
    Motor_init();                                   //Initializes the dc motor.
    Timer_A_servo_init();                           //Initializes the servo motors timer.
    Servo_motor_init();                             //Initializes the servo motor.
    Lights_init();                                  //Initializes the RGB LED's.
    Timer_A_lights_init();                          //Initializes the RGB LED's timer.

    Main_Menu();                                    //Calls the "Main_Menu" function, to be displayed on the LCD.

    IRQ_pin_init();                                 //Initializes the interrupts required ports and pins.
    Timer_A_LCD_init();                             //Initializes the LCD's timer, to be used for its brightness.
    ADC_init();                                     //Initializes the ADC conversion for the LCD's brightness.

    NVIC_EnableIRQ(PORT1_IRQn);         //Enable interrupt in NVIC vector.
    NVIC_EnableIRQ(PORT6_IRQn);         //Enable interrupt in NVIC vector.
    NVIC_EnableIRQ(T32_INT1_IRQn);      //Enable interrupt in NVIC vector.

    __enable_irq();                     //Enable global interrupt.

    Timer_32_init();                    //Initializes the Timer_32 to be used for interrupts.

    while(1)

    {

        delay_ms(5);      //5 millisecond delay.
        Menu_Loop();      //Calls "Menu_Loop" function.

    }

 }
