/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/04/2021

Project:        LAB10_Part2

File:           mainPart2.c

Professor:      Trevor Ekin

Description:    For part 2, the same code is used, but a temperature sensor is
                used in place of a potentiometer, so that the actual room temp
                is displayed in the console window of Code Composer Studio.

*/

#include "msp.h"
#include <stdio.h>

void ADC_init(void);        //Prototype function that initializes analog input channel A0 using port 5 pin 5, which is configured for Analog-to-digital conversion.
void SysTick_init(void);        //Prototype function for SysTick initialization.
void SysTick_delay(uint16_t delay);     //Prototype function for delays in milliseconds using the internal SysTick timer.

float volt;
float temp;

uint16_t result;

void main(void)

{
    ADC_init();

    SysTick_init();

    while(1)

    {

        ADC14 -> CTL0 |= 1;       //Starts a conversion.

        while(!ADC14 -> IFGR0);       //Will wait until conversion is completed.

        result = ADC14 -> MEM[0];     //Immediately stores the value into a variable.

        volt = (result * 3.3) / 16384;

        temp = (volt - 0.5) / 0.01;

        printf("Current Temp. is:\n\t%.1f\n", temp);

        SysTick_delay(1000);

    }

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
