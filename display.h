#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DISPLAY_HEIGHT 96
#define DISPLAY_WIDTH 96

#include "peripherals.h"
#include "note.h"

typedef char DisplayRow;
// DisplayRow Bit Layout
// 0000 0000
// ||||_Placement
// Spacing
// Lower 4 bits represent the placement of a black box on the screen.
// If one of the lower 4 bits is a 1, the corresponding position on the
// screen will have a black box drawn.
// Upper 4 bits represent vertical spacing between the black boxes.
// If one of the upper 4 bits is a 1, the corresponding box on the
// screen will be slightly shorter (have blank space above)
// which indicates a different note.

void print_str(char *str, int32_t x, int32_t y);
void show_print(void);
void clear_display(void);

void drawRowsDirect(DisplayRow *rows, unsigned int num_rows);
void calcRows(Note *notes, DisplayRow *output, unsigned int max_rows, unsigned int skip_rows);

void displayNotes(Note* notes);

#endif /* DISPLAY_H_ */
