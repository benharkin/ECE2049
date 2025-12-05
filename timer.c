#include <msp430.h>
#include "timer.h"

// This is static, so other source files will not be able to access it directly,
// for encapsulation.

static volatile unsigned long timer;

void setupTimer(void)
{
    //Timer A2 setup for interrupts every 1/128 second
    TA2CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TA2CCR0 = (32768 / TICKSPS) - 1;
    TA2CCTL0 = CCIE;
}

// Returns the time in multiples of 1/TICKSPS seconds
unsigned long getTime(void)
{
//    __disable_interrupt();
//    unsigned long temp = timer;
//    __enable_interrupt();
    return timer;
}

// Returns the time in seconds
float getSecondsF(void)
{
    return getTime() / (float) TICKSPS;
}

unsigned long getSeconds(void)
{
    return getTime() / TICKSPS;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void)
{
    timer++;
    //TA2CCTL0 &= ~CCIFG;
}

timedate_t getTimeDate(void)
{
    return secondsToTimeDate(getSeconds());
}

void setTimer(unsigned long ticks)
{
    timer = ticks;
}
void setSeconds(unsigned long seconds)
{
    timer = seconds * TICKSPS;
}
void setTime(timedate_t time)
{
    setTimer(timeDateToSeconds(time));
}

timedate_t secondsToTimeDate(unsigned long seconds)
{
    unsigned char minutes = seconds / 60;
    unsigned char hours = minutes / 60;
    unsigned char days = hours / 24;
    unsigned char months = days / 30;
    timedate_t out = { months % 12, days % 30, hours % 24, minutes % 60, seconds
                               % 60 };
    return out;

}

unsigned long timeDateToSeconds(timedate_t td)
{
    unsigned long out = td.second;
    out += td.minute * 60;
    out += td.hour * 3600;
    out += td.day * 86400;
    out += td.month * 2592000;
    return out;
}
