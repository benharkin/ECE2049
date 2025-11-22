#include <msp430.h>
#include "timer.h"

// This is static, so other source files will not be able to access it directly,
// for encapsulation.

static volatile unsigned long timer;

void setupTimer(void){
    //Timer A2 setup for interrupts every 1/128 second
    TA2CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TA2CCR0 = (32768 / TICKSPS) - 1;
    TA2CCTL0 = CCIE;
}

// Returns the time in multiples of 1/TICKSPS seconds
unsigned long getTime(void){
//    __disable_interrupt();
//    unsigned long temp = timer;
//    __enable_interrupt();
    return timer;
}

// Returns the time in seconds
float getSecondsF(void){
    return getTime() / (float)TICKSPS;
}

unsigned int getSeconds(void){
    return getTime() / TICKSPS;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void)
{
    timer++;
    //TA2CCTL0 &= ~CCIFG;
}

