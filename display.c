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
        buf[i] = num % 10;
        i++;
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
    char date[] = "MMM DD";
    char time[] = "HH:MM:SS";

    strncpy(date, monthNames[td.month -1], 3); // Fill the month
    numToCharBuf(td.day, date + 4, 2); // Fill the day

    numToCharBuf(td.hour, date, 2);
    numToCharBuf(td.minute, date + 3, 2);
    numToCharBuf(td.second, date + 6, 2);

    print_str(date, 48, 48, NOCLEAR);
    print_str(time, 48, 32, CLEAR);

}

void displayTemp(float inAvgTempC)
{
    float avgTempF = 1.8 * inAvgTempC + 32;

    int tempC = (int) (inAvgTempC * 10); // Stored in tenths of a degree C
    int tempF = (int) (avgTempF * 10); // Stored in tenths of a degree F

    char temp[] = "XXX.X C, XXX.X F";
    // Fill the integer part of C
    numToCharBuf(tempC / 10, temp, 3);
    // Fill the decimal part of C
    numToCharBuf(tempC % 10, temp + 4, 1);
    // Fill the integer part of F
    numToCharBuf(tempF / 10, temp + 9, 3);
    // FIll the decimal part of C
    numToCharBuf(tempF % 10, temp + 13, 1);

    print_str(temp, 48, 48, CLEAR);
}
