#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "peripherals.h"
#include "timer.h"

#define DISPLAY_HEIGHT 96
#define DISPLAY_WIDTH 96

#define NOCLEAR 0
#define CLEAR 1
void print_str(char *str, int32_t x, int32_t y, char clear);
void numToCharBuf(int num, char *buf, unsigned int buf_len);
void displayTime(timedate_t td);
void displayDate(timedate_t td);
void displayTemp(float temp, char unit);

#endif /* DISPLAY_H_ */
