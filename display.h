#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "peripherals.h"
#include "timer.h"

#define DISPLAY_HEIGHT 96
#define DISPLAY_WIDTH 96

#define NOCLEAR 0
#define CLEAR 1
void print_str(char *str, int32_t x, int32_t y, char clear);

void displayTime(timedate_t td);



#endif /* DISPLAY_H_ */
