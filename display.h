/*
 * display.h
 *
 *  Created on: Nov 18, 2025
 *      Author: Wwint
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "peripherals.h"
#include "note.h"

void print_str(char *str, int32_t x, int32_t y);
void show_print(void);
void clear_display(void);

void displayNotes(Note* notes);

#endif /* DISPLAY_H_ */
