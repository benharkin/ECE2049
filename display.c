#include "display.h"

void print_str(char *str, int32_t x, int32_t y);
void clear_display(void);




void print_str(char *str, int32_t x, int32_t y)
{
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                x, y, TRANSPARENT_TEXT);
}

void show_print(void){
    Graphics_flushBuffer(&g_sContext);
}

void clear_display(void)
{
    Graphics_clearDisplay(&g_sContext);
    Graphics_flushBuffer(&g_sContext);
}



void displayNotes(Note* notes){
    // array to hold the output for the screen
    // Need LEDs of note, and duration later
    int16_t quarters[4];

    for(int i = 0; i < 4; i++){
        Note thisNote = notes[i];

        char position_bit = getLED(thisNote); // Bitwise position for printing
        int16_t position;
        if(position_bit == BIT0)
            position = 0;
        else if(position_bit == BIT1)
            position = 1;
        else if(position_bit == BIT2)
            position = 2;
        else
            position = 3;
        quarters[i] = position;

        if(!isEnd(thisNote)){
            int height = getDuration(thisNote)/1000;
            i += height-1;

            for(int j = height-1; j >= 0; j--){
                quarters[j+i] = position;
            }
        }
        else{
            while(i<4){
                quarters[i] = 5;
                i++;
            }
        }
    }
    // display array


    // Bottom Note
    Graphics_Rectangle note0 = {quarters[0]*24, 0, (quarters[0]+1)*24, 24};
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note0, 0);
//    Graphics_fillRectangleOnDisplay(((&g_sContext)->Graphics_Display, note0, 0);
    // Next Note
    Graphics_Rectangle note1 = {quarters[1]*24, 25, (quarters[1]+1)*24, 48};
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note1, 0);
    // Third Note
    Graphics_Rectangle note2 = {quarters[2]*24, 49, (quarters[2]+1)*24, 72};
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note2, 0);
    // Top Note
    Graphics_Rectangle note3 = {quarters[3]*24, 73, (quarters[3]+1)*24, 96};
    Graphics_fillRectangleOnDisplay((&g_sContext)->display, &note3, 0);

    show_print();

}
