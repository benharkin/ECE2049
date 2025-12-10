#include "display.h"
#include <string.h>

static char *monthNames[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                              "Aug", "Sep", "Oct", "Nov", "Dec" };
static char* carets = "^^^^^^^^^^";

void print_str(char *str, int32_t x, int32_t y, char clear)
{

    if (clear)
        Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                x, y, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void print_str_emph(char *str, int32_t x, int32_t y, char clear, edit_t mode){
    if (clear)
            Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                    x, y, TRANSPARENT_TEXT);
    if(mode != NONE)
    {
        int start = get_min_highlight(mode);
        int end = get_max_highlight(mode);
        int length = end-start+1;

        int str_begin_coord = x - Graphics_getStringWidth(&g_sContext, str, AUTO_STRING_LENGTH) / 2 + 5*start;
        Graphics_drawString(&g_sContext, carets, length, str_begin_coord, y + 10, TRANSPARENT_TEXT);
    }
    Graphics_flushBuffer(&g_sContext);

}

// Fill a char buf with the number's digits- NOT NULL TERMINATED!!
void numToCharBuf(int num, char *buf, unsigned int buf_len)
{
    // Write the number to the buffer in reverse.
    for (int i = 0; i < buf_len; i++)
    {
        buf[i] = (char) (num % 10 + '0');
        num /= 10;
    }

    // Reverse the numbers written so the greatest is first.
    // Zeros will precede the number if there are not enough digits.
    int start = 0;
    int end = buf_len - 1;
    while (start < end)
    {
        char temp = buf[start];
        buf[start] = buf[end];
        buf[end] = temp;
        start++;
        end--;
    }
}

void displayTime(timedate_t td, edit_t mode)
{
    char time[] = "HH:MM:SS";
    numToCharBuf(td.hour, time, 2);
    numToCharBuf(td.minute, time + 3, 2);
    numToCharBuf(td.second, time + 6, 2);
    print_str_emph(time, 48, 48, CLEAR, mode);
}

void displayDate(timedate_t td, edit_t mode)
{
    char date[] = "MMM DD";
    strncpy(date, monthNames[td.month - 1], 3); // Fill the month
    numToCharBuf(td.day, date + 4, 2); // Fill the day
    print_str_emph(date, 48, 48, CLEAR, mode);
}

void displayTemp(float temp, char unit)
{
    int tenths = (int)(temp*10);
    char out[] = "XXX.X X";
    out[6] = unit;
    numToCharBuf(tenths / 10, out, 3); // Integer Part
    numToCharBuf(tenths % 10, out + 4, 1); // Decimal Part
    print_str(out, 48, 48, CLEAR);
}

int get_min_highlight(edit_t mode){
    switch (mode){
    case MONTH:
        return 0;
    case DAY:
        return 5;
    case HOUR:
        return 0;
    case MIN:
        return 4;
    case SEC:
        return 7;
    default:
        return -1; // Return flag
    }
}

int get_max_highlight(edit_t mode){
    switch (mode){
    case MONTH:
        return 2;
    case DAY:
        return 6;
    case HOUR:
        return 1;
    case MIN:
        return 5;
    case SEC:
        return 8;
    default:
        return -1; // Return flag
    }
}
