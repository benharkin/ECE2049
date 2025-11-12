/*
 * note.h
 *
 *  Created on: Nov 12, 2025
 *      Author: ben
 */

#ifndef NOTE_H_
#define NOTE_H_

// Had to append 0 to all of them because there were macro redefinition conflicts.
#define A0 0
#define B0 1
#define C0 2
#define D0 3
#define E0 4
#define F0 5
#define G0 6
#define A1 7

// These are the denominator - 1. So a whole note has a denominator of 1,
// so it is defined here as 0. Shifted right 3 to make room for the pitch bits
#define WN (0 << 3)
#define HN (1 << 3)
#define QN (3 << 3)
#define EN (7 << 3)
#define SN (15 << 3)
#define TN (31 << 3)

// Usage:
// A0 | HN is an A0 half note.

// Note type, since we are using a special bit layout
typedef char Note;

// Returns the period of the note in quartz ticks
int getPeriod(Note n);

// Returns the length of the note in quartz ticks
int getDuration(Note n);

// Returns a char with the least significant 4 bits representing the status of the LEDS for the note.
// The LSB is the leftmost and the MSB is the rightmost. Can change this if necessary.
char getLED(Note n);

#endif /* NOTE_H_ */
