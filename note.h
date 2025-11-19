/*
 * note.h
 *
 *  Created on: Nov 12, 2025
 *      Author: ben
 */

#ifndef NOTE_H_
#define NOTE_H_
#define WHOLE_NOTE 2500 // So a whole note is 4 seconds


// Note
// 0000 0000
// |||| |^^^
// ||||_|Pitch
// |||Duration
// ||Rest
// Unused

typedef char Note;

// Had to append 0 to all of them because there were macro redefinition conflicts.
#define A0 0
#define B0 1
#define C0 2
#define D0 3
#define E0 4
#define F0 5
#define G0 6
#define A1 7

// These are the denominator. So a whole note has a denominator of 1.
// Shifted right 3 to make room for the pitch bits. These bits BIT3/BIT4
// must be set or the note will be considered the null terminator.
#define WN (1 << 3)
#define HN (2 << 3)
#define QN (3 << 3)

// If BIT4 (5th bit) is 1, the note is silent.
#define REST BIT5

// Usage:
// A0 | HN is an A0 half note.
// REST | HN is a half note rest
// 0 is the null terminator note


// Returns the period of the note in quartz ticks
int getPeriod(Note n);

// Returns the length of the note in ms
int getDuration(Note n);

// Returns if this note is a rest.
int isRest(Note n);

// Returns if the note is the null terminator (0) equivalent to n == 0
int isEnd(Note n);

// Returns a char with the least significant 4 bits representing the status of the LEDS for the note.
// The LSB is the leftmost and the MSB is the rightmost. Can change this if necessary.
char getLED(Note n);

#endif /* NOTE_H_ */
