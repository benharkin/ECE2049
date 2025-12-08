#include "display.h"
#include <string.h>

static char *monthNames[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                              "Aug", "Sep", "Oct", "Nov", "Dec" };

void print_str(char *str, int32_t x, int32_t y, char clear)
{

    if (clear)
        Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                x, y, TRANSPARENT_TEXT);
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

void displayTime(timedate_t td)
{
    char time[] = "HH:MM:SS";
    numToCharBuf(td.hour, time, 2);
    numToCharBuf(td.minute, time + 3, 2);
    numToCharBuf(td.second, time + 6, 2);
    print_str(time, 48, 48, CLEAR);
}

void displayDate(timedate_t td)
{
    char date[] = "MMM DD";
    strncpy(date, monthNames[td.month - 1], 3); // Fill the month
    numToCharBuf(td.day, date + 4, 2); // Fill the day
    print_str(date, 48, 48, CLEAR);
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
