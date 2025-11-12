/*
 * note.c
 *
 *  Created on: Nov 12, 2025
 *      Author: ben
 */
#include "note.h"

// These defines are local to this file
#define QUARTZ 32768
#define WHOLE_NOTE 4*QUARTZ // So a whole note is 4 seconds worth of ACLK ticks

// Note is a byte where the 3 least significant bits represent the pitch
// and the 5 most significant bits represent the denominator of the note.
// That is, an eighth note will have the 5 MSB equal to 8.

// Return the period of the note in ACLK ticks (32768 Hz)
int getPeriod(Note n){
    static const int[] pitch_map = {73, 65, 62, 55, 49, 46, 41 ,36};
    // 73 is the period in ACLK ticks of the A0, 36 is A1
    const int[] map = {73, 65, 62, 55, 49, 46, 41 ,36};
    char pitch = n & (BIT0|BIT1|BIT3); // Get the 3 LSB
    return pitch_map[pitch];
}

int getDuration(Note n){
    char denominator = n >> 3; // Discard the 3 LSB
    return WHOLE_NOTE / (denominator + 1);
}

char getLED(Note n){
    static const char[] LED_map = {BIT0, BIT1, BIT2, BIT3, BIT0, BIT1, BIT2, BIT3};
    char pitch = n & (BIT0|BIT1|BIT3); // Get the 3 LSB
    return LED_map[pitch];
}

