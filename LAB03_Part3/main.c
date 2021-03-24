#include "msp.h"

void main(void)

{

    //Stop watch dog timer (WDT is used to break you out of unwanted loops, more on this later in the semester)

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    //Configure GPIO for output on P1.0 LED1 on MSP432 Launchpad

    P1->DIR = BIT0;

    //Temporary variable for loop-maintenance

    int delay;
    int i;

    delay = 40000;

    while(1)

    {
        P1->OUT ^= BIT0;        //Toggle LED status
        for(i=delay; i>0; i--); //Crude delay
    }
}
