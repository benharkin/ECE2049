#include "display.h"

char getPlacement(DisplayRow r)
{
    return r & (BIT3 | BIT2 | BIT1 | BIT0);
}

char getSpacing(DisplayRow r)
{
    return r >> 4;
}

char getSolidRects(DisplayRow r)
{
    return getPlacement(r) & ~getSpacing(r);
}

char getSpacedRects(DisplayRow r)
{
    return getPlacement(r) & getSpacing(r);
}

void calcRows(Note *notes, DisplayRow *output, unsigned int max_rows, unsigned int skip_rows)
{
    int rows_written = 0;
    int row = 0;
    for (int note_index = 0; !isEnd(notes[note_index]); note_index++)
    {
        Note this_note = notes[note_index];

        char h_pos = getLED(this_note);
        int height = getEighths(this_note);
        int show_note = !isRest(this_note);

        height -= skip_rows; // Shrink the note's height
        if(height < 0){
            skip_rows -= height; // Add back height that could not be subtracted while remaining positive
        }

//        for(int row = rows_written; row < rows_written + height && row < max_rows; row++){
//            DisplayRow output_row = 0;
//            // Height will be positive here due to the for loop condition.
//
//        }

        while(row < max_rows && height > 0){
            DisplayRow output_row = 0;
            //if(height)

            row++;
            height--;
        }


//        if(skip_rows > 0){
//            skip_rows -= duration;
//            continue;
//        }
//        for (int j = 0; j + row_offset < max_rows && j < duration - skip_rows; j++)
//        {
//            DisplayRow result = 0;
//            if (j == 0 && !rest)
//            { // First section of a note
//                result |= (note_pos << 4);
//            }
//            if (!rest)
//            { // Note should be drawn
//                result |= note_pos;
//            }
//            output[j + row_offset] = result;
//        }
//
//        row_offset += duration; // Maybe
    }
}

void drawRowsDirect(DisplayRow *rows, unsigned int num_rows)
{
    const Graphics_Display *display = g_sContext.display;
    Graphics_clearDisplay(&g_sContext);
    for (int i = 0; i < num_rows; i++)
    {
        DisplayRow this_row = rows[i];
        for (int j = 0; j < 3; j++)
        {
            char draw_this_col = (this_row >> j) & BIT0;
            char space_this_col = (this_row >> (j + 4)) & BIT0;
            int k = 3 - j; // k is the distance from the left of the screen, j is from the right.

            if (draw_this_col)
            {
                int xmin = k * 24 + 4;
                int xmax = (k + 1) * 24 - 4;
                int ymin = i * 12 - (space_this_col * 2);
                int ymax = (i + 1) * 12;
                const Graphics_Rectangle rect = { xmin, ymin, xmax, ymax };
                Graphics_fillRectangleOnDisplay(display, &rect, 0);
            }
        }
    }
    Graphics_flushBuffer(&g_sContext);
}

void print_str(char *str, int32_t x, int32_t y)
{
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                x, y, TRANSPARENT_TEXT);
}

void show_print(void)
{
    Graphics_flushBuffer(&g_sContext);
}

void clear_display(void)
{
    Graphics_clearDisplay(&g_sContext);
    Graphics_flushBuffer(&g_sContext);
}

void displayNotes(Note *notes)
{
    // array to hold the output for the screen
    // Need LEDs of note, and duration later
    int16_t quarters[4];

    for (int i = 0; i < 4; i++)
    {
        Note thisNote = notes[i];

        char position_bit = getLED(thisNote); // Bitwise position for printing
        int16_t position;
        if (position_bit == BIT0)
            position = 0;
        else if (position_bit == BIT1)
            position = 1;
        else if (position_bit == BIT2)
            position = 2;
        else if (position_bit == BIT3)
            position = 3;
        else
            position = 5;
        quarters[i] = position;

        if (!isEnd(thisNote))
        {
            int height = getDuration(thisNote) / (WHOLE_NOTE / 4);
            i += height - 1;

            for (int j = height - 1; j >= 0; j--)
            {
                quarters[j + i] = position;
            }
        }
        else
        {
            while (i < 4)
            {
                quarters[i] = 5;
                i++;
            }
        }
    }
    // display array

    clear_display();
    // Bottom Note
    Graphics_Rectangle note0 =
            { quarters[0] * 24, 0, (quarters[0] + 1) * 24, 24 };
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note0, 0);
//    Graphics_fillRectangleOnDisplay(((&g_sContext)->Graphics_Display, note0, 0);
    // Next Note
    Graphics_Rectangle note1 = { quarters[1] * 24, 25, (quarters[1] + 1) * 24,
                                 48 };
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note1, 0);
    // Third Note
    Graphics_Rectangle note2 = { quarters[2] * 24, 49, (quarters[2] + 1) * 24,
                                 72 };
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note2, 0);
    // Top Note
    Graphics_Rectangle note3 = { quarters[3] * 24, 73, (quarters[3] + 1) * 24,
                                 96 };
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note3, 0);

    show_print();

}
