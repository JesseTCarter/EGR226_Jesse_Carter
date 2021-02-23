#include "msp.h"

void pin_init(void);
uint8_t DebounceSwitch1(void);

void main(void)

{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    pin_init();

    int color = 5;

    while(1)

    {

        if(DebounceSwitch1())

        {

            P3OUT &= BIT(color);                //Clear Port 3 Bit color
            P3OUT ^= BIT(color);                //Toggle Port 3 Bit color
            if(color == 7) color = 4;           //Sets color back to 4
            color++;

        }

    }

}

uint8_t DebounceSwitch1(void)                   //Switch on Port P2.5

{

    static uint16_t State = 0;
    State = (State<<1) | ((P2IN & BIT5)>>1) | 0xf800;
    __delay_cycles(5);

    if(State == 0xfc00)

        return 1;

    return 0;

}

void pin_init(void)

{

    P2->SEL1 &= ~BIT5;                                          /* configure P2.5 as simple I/O */
    P2->SEL0 &= ~BIT5;
    P2->DIR &= ~BIT5;                                           /*P2.5 set as input */
    P2->REN |= BIT5;                                            /*P2.5 pull resistor enabled */
    P2->OUT |= BIT5;                                            /* Pull up/down is selected by P1 ->OUT */

    P3->SEL0 &= ~BIT5;                                          /* configure P3.5 as simple I/O */
    P3->SEL1 &= ~BIT5;
    P3->DIR |= BIT5;                                            /* set P3.5 as output pin */
    P3->OUT &= BIT5;

    P3->SEL0 &= ~BIT6;                                          /* configure P3.5 as simple I/O */
    P3->SEL1 &= ~BIT6;
    P3->DIR |= BIT6;                                            /* set P3.5 as output pin */
    P3->OUT &= BIT6;

    P3->SEL0 &= ~BIT7;                                          /* configure P3.5 as simple I/O */
    P3->SEL1 &= ~BIT7;
    P3->DIR |= BIT7;                                            /* set P3.5 as output pin */
    P3->OUT &= BIT7;

}
