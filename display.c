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
    char quarters[4];

    for(int i = 0; i < 4; i++){
        Note thisNote = notes[i];

//        if(isEnd(thisNote)){
//            break;
//        }

        int height = getDuration(thisNote)/1000;
        i += height-1;
        char position_bit = getLED(thisNote); // Bitwise position for printing
        int position;
        if(position_bit == BIT0)
            position = 0;
        else if(position_bit == BIT1)
            position = 1;
        else if(position_bit == BIT2)
            position = 2;
        else(position_bit == BIT3)
            position = 3;



        for(int j = height; j >= 0; j--){
            quarters[j] = position;
        }
    }
    // display array
    // Bottom Note
    struct Graphics_Rectangle note1 = {position*24, y min, (position+1)*24, ymax}
    Graphics_fillRectangleOnDisplay((&g_sContext.Graphics_Display, const Graphics_Rectangle *rect, uint16_t value);
    // Next Note
    Graphics_fillRectangleOnDisplay(const Graphics_Display *display,
                const Graphics_Rectangle *rect, uint16_t value);
    // Third Note
    Graphics_fillRectangleOnDisplay(const Graphics_Display *display,
                const Graphics_Rectangle *rect, uint16_t value);
    // Top Note
    Graphics_fillRectangleOnDisplay(const Graphics_Display *display,
                const Graphics_Rectangle *rect, uint16_t value);


}
