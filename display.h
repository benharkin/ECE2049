#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "peripherals.h"
#include "timer.h"

#define DISPLAY_HEIGHT 96
#define DISPLAY_WIDTH 96

typedef enum
{
    MONTH, DAY, HOUR, MIN, SEC, NONE
} edit_t;

#define NOCLEAR 0
#define CLEAR 1
void print_str(char *str, int32_t x, int32_t y, char clear);
void print_str_emph(char *str, int32_t x, int32_t y, char clear, edit_t mode);
void numToCharBuf(int num, char *buf, unsigned int buf_len);
void displayTime(timedate_t td, edit_t mode);
void displayDate(timedate_t td, edit_t mode);
void displayTemp(float temp, char unit);
int get_max_highlight(edit_t mode);
int get_min_highlight(edit_t mode);

#endif /* DISPLAY_H_ */
