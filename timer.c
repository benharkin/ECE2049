#include <msp430.h>
#include "timer.h"

// This is static, so other source files will not be able to access it directly,
// for encapsulation.

static volatile unsigned long timer;

void setupTimer(void){
    //Timer A2 setup for interrupts every 5 ms
    TA2CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TA2CCR0 = 163;
    TA2CCTL0 = CCIE;
}

// Returns the time in multiples of ~1ms
unsigned long getTime(void){
    __disable_interrupt();
    unsigned long temp = timer * 5;
    __enable_interrupt();
    return temp;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void)
{
    timer++;
    TA2CCTL0 &= ~CCIFG;
}

