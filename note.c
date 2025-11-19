/*
 * note.c
 *
 *  Created on: Nov 12, 2025
 *      Author: ben
 */
#include <msp430.h>
#include "note.h"

// These defines are local to this file
//#define QUARTZ 32768
#define WHOLE_NOTE 4000 // So a whole note is 4 seconds

#define PITCH(n) n & (BIT0|BIT1|BIT2)
#define DURATION(n) (n & (BIT3|BIT4)) >> 3
#define REST(n) (n & BIT5) >> 5
#define END(n) n == 0

// Return the period of the note in ACLK ticks (32768 Hz)
int getPeriod(Note n){
    // 73 is the period in ACLK ticks of the A0, 36 is A1
    static const int pitch_map[] = {73, 65, 62, 55, 49, 46, 41 ,36};
    return pitch_map[PITCH(n)];
}

int getDuration(Note n){
    char denominator = DURATION(n);
    if(denominator == 0){ // If note is null terminator
        return 0;
    }
    return WHOLE_NOTE / denominator;
}

int isRest(Note n){
    return REST(n);
}

int isEnd(Note n){
    return END(n);
}

char getLED(Note n){
    static const char LED_map[] = {BIT0, BIT1, BIT2, BIT3, BIT0, BIT1, BIT2, BIT3};
    return LED_map[PITCH(n)];
}

