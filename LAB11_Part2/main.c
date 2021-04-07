/*

Name:           Jesse Carter

Course:         EGR 226-902

Date:           04/07/2021

Project:        LAB11_Part2

File:           mainPart2.c

Professor:      Trevor Ekin

Description:    Part 2 is programmed to generate an IR signal and has
                that signal detected be an IR receiver, the on-board
                red LED is on, and when the signal is blocked, the
                LED turns off.

*/

#include "msp.h"

unsigned short currentedge;
unsigned short lastedge;
unsigned short period;

uint8_t detect10Hz = 1; //Starts red on-board LED in on state.

void TimerA0Init(void);     //Prototype function for TimerA register initialization.
void Pin_init(void);        //Prototype function for pin initialization.

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //Stop watchdog timer.

    Pin_init();

    TimerA0Init();

    NVIC->ISER[0] = 1<<((TA2_N_IRQn) & 31);     //Enable interrupt in NVIC vector.

    __enable_irq();     //Enable global interrupt.

    while(1)

        if(detect10Hz)

        {

            P1->OUT ^= BIT0;        //Toggle red on-board LED.

            detect10Hz = 0;     //Set the detect10Hz variable back to 0

        }

}

/*

                       | TimerA0Init function |

        Brief: TimerA0Init initializes all of the
               corresponding timer CCR, CCTL, and
               CTL registers.

        parameters: N/A

        return: N/A

*/

void TimerA0Init(void)

{

    TIMER_A0 -> CCR[0] = 37500;       //10 Hz.

    TIMER_A0 -> CCR[1] = 37500 / 2;       //50% duty cycle.

    TIMER_A0 -> CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    TIMER_A0 -> CTL |= TIMER_A_CTL_TASSEL_2|      //Use SMCLK as clock source

                       TIMER_A_CTL_ID_3|      //1/8 input divider (min frequency of 5.7 Hz)

                       TIMER_A_CTL_MC__UP|        //Start timer in UP mode

                       TIMER_A_CTL_CLR;       //clear TA0R

    TIMER_A2 -> CTL |= TIMER_A_CTL_TASSEL_2|      //Use SMCLK as clock source

                       TIMER_A_CTL_MC__UP|        //Start timer in UP mode

                       TIMER_A_CTL_ID_3|      //Divide by 8

                       TIMER_A_CTL_CLR;       //Clear TA2R

    TIMER_A2 -> CCTL[2] = TIMER_A_CCTLN_CM_1|     //Capture rising edge

                          TIMER_A_CCTLN_CCIS_0|       //Use CCI2A = ACLK

                          TIMER_A_CCTLN_CCIE|     //Enable capture interrupt

                          TIMER_A_CCTLN_CAP|      //Enable capture mode

                          TIMER_A_CCTLN_SCS;      //Synchronous capture

}

/*
                       | Pin_init function |

        Brief: The Pin_init function initializes the required
               ports and pins on the MSP432 for the program.

        parameters: N/A

        return: N/A

*/

void Pin_init(void)

{

   P2->SEL0 |= BIT4;        // P2.4 set TA0.1.
   P2->SEL1 &=~ BIT4;
   P2->DIR |= BIT4;

   P5->SEL0 |=  BIT7;       //P5.7 Set TA2.2.
   P5->SEL1 &= ~BIT7;
   P5->REN  |= BIT7;        //Pull resistor enabled.


   P1->SEL0 &= ~BIT0;       //Configuring red on-board LED.
   P1->SEL0 &= ~BIT0;       //Set P1.0 as GPIO.
   P1->DIR  |=  BIT0;       //Set direction to output.
   P1->OUT  &= ~BIT0;       //Set output to low.

}

/*
                       | TA2_N_IRQHandler function |

        Brief: The TA2_N_IRQHandler function initializes the
               required timerA handler that will receive the
               IR signal.

        parameters: N/A

        return: N/A

*/

void TA2_N_IRQHandler(void)

{

    TIMER_A0 -> CCTL[2] &= ~1;      //Clearing TA.2 interrupt flag.

    currentedge = TIMER_A2->CCR[2];

    period = currentedge - lastedge;

    lastedge = currentedge;

    TIMER_A2->CCTL[2] &= ~(TIMER_A_CCTLN_CCIFG);        //Reset interrupt flag.

    detect10Hz = 1;

}
