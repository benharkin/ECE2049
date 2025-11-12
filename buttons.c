#include <msp430.h>
#include "buttons.h"
void initButtons()
{
    // Configure P7.0 for digital input. Corresponds to S1
    P7SEL &= ~BIT0; // Digital I/O
    P7DIR &= ~BIT0; // Input
    P7REN |= BIT0; // Pullup/down enable
    P7OUT |= BIT0; // Set pullup

    // Configure P3.6 for digital input. Corresponds to S2
    P3SEL &= ~BIT6;
    P3DIR &= ~BIT6;
    P3REN |= BIT6;
    P3OUT |= BIT6;

    // P2.2 (S3)
    P2SEL &= ~BIT2;
    P2DIR &= ~BIT2;
    P2REN |= BIT2;
    P2OUT |= BIT2;

    // P7.4 (S4)
    P7SEL &= ~BIT4;
    P7DIR &= ~BIT4;
    P7REN |= BIT4;
    P7OUT |= BIT4;
}

char getButtons()
{
    char s1 = ~P7IN & BIT0; // 0th Bit
    char s2 = (~P3IN & BIT6) >> 5; // 1st bit
    char s3 = (~P2IN & BIT2); // 2nd bit
    char s4 = (~P7IN & BIT4) >> 1; //3rd bit
    return s1 | s2 | s3 | s4;
}
