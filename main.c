#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"
#include "timer.h"
#include "display.h"
#include "sensor.h"

typedef enum
{
    EDIT, DATE, TIME, TEMP_C, TEMP_F
} state_t;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    _BIS_SR(GIE); // Global interrupts enable
    setupTimer();
    setupADC();


    configDisplay();
    configKeypad();
    initButtons();

    state_t state = DATE;
    edit_t edit_mode = NONE;

    char key;
    char prevKey = 0;
    char prev_butt = 0;
    short int count = 0;
    unsigned long prev_time = 0;

    //Initial time
    timedate_t init_timedate = {6,27,0,0,0};
    setTime(init_timedate);

    unsigned long time;
    timedate_t timedate;
    timedate_t edittimedate;
    timedate_t temptimedate;



    while (1)
    {
        prevKey = key;
        prev_time = time;
        key = getKey();
        time = getTime();
        timedate = secondsToTimeDate(time);


        if(time != prev_time)
        {
            // Update temp
            updateTempAverage(time);
        }

        // # key switches between edit and not edit
        if (key != prevKey && key == '#')
        {
            if(state == EDIT)
            {
                state = DATE;
                edit_mode = NONE;
                setTime(temptimedate);
            }
            else
            {
                state = EDIT;
                edit_mode = MONTH;
                //edittimedate = timedate;
                temptimedate = timedate;
            }
        }

        switch (state)
        {
        case DATE:
        {
            //Display the date each second for 3 seconds
            if (time%3 == count)
            {
                displayDate(timedate, edit_mode);
                count++;
            }
            if(count == 3)
            {
                count = 0;
                state = TIME;
            }
            break;
        }
        case TIME:
        {
            //Display the time each second for 3 seconds
            if (time%3 == count)
            {
                displayTime(timedate, edit_mode);
                count++;
            }
            if(count == 3)
            {
                count = 0;
                state = TEMP_C;
            }
            break;
        }
        case TEMP_C:
        {
            //Display the temp in C each second for 3 seconds
            if (time%3 == count)
            {
                displayTemp(get_Temp_AVG_C(),'C');
                count++;
            }
            if(count == 3)
            {
                count = 0;
                state = TEMP_F;
            }
            break;

        }
        case TEMP_F:
        {
            //Display the temp in C each second for 3 seconds
            if (time%3 == count)
            {
                displayTemp(get_Temp_AVG_F(),'F');
                count++;
            }
            if(count == 3)
            {
                count = 0;
                state = DATE;
            }
            break;
        }
        case EDIT:
        {

            char newbutt = getButtons();
            char butt;
            if(newbutt == prev_butt)
                butt = 0;
            else
                butt = newbutt;
            prev_butt = newbutt;

            switch(edit_mode)
            {
            case MONTH:
            {
                // Set temporary month to potentiometer if new month read
                unsigned char pot = get_pot_scaled(1, 12);

                if(temptimedate.month != pot)
                {
                    temptimedate.month = pot;
                    displayDate(temptimedate, edit_mode); // Highlight Month
                }

                if(butt == BIT0)
                {
                    // Left Button Pressed
                    edittimedate.month = temptimedate.month;
                    edit_mode = SEC;
                }

                if(butt == BIT1)
                {
                    // Right Button Pressed
                    edittimedate.month = temptimedate.month;
                    edit_mode = DAY;
                }
                break;

            }

            case DAY:
            {
                unsigned char pot = get_pot_scaled(1, 31);

                if(temptimedate.day != pot)
                {
                    temptimedate.day = pot;
                    displayDate(temptimedate, edit_mode); // Highlight Day
                }

                if(butt == BIT0)
                {
                    // Left Button Pressed
                    edittimedate.day = temptimedate.day;
                    edit_mode = MONTH;
                }

                if(butt == BIT1)
                {
                    // Right Button Pressed
                    edittimedate.day = temptimedate.day;
                    edit_mode = HOUR;
                }
                break;
            }

            case HOUR:
            {
                unsigned char pot = get_pot_scaled(1, 24);

                if(temptimedate.hour != pot)
                {
                    temptimedate.hour = pot;
                    displayTime(temptimedate, edit_mode); // Highlight Hour
                }

                if(butt == BIT0)
                {
                    // Left Button Pressed
                    edittimedate.hour = temptimedate.hour;
                    edit_mode = DAY;
                }

                if(butt == BIT1)
                {
                    // Right Button Pressed
                    edittimedate.hour = temptimedate.hour;
                    edit_mode = MIN;
                }
                break;
            }

            case MIN:
            {
                unsigned char pot = get_pot_scaled(1, 59);

                if(temptimedate.minute != pot)
                {
                    temptimedate.minute = pot;
                    displayTime(temptimedate, edit_mode); // Highlight Min
                }

                if(butt == BIT0)
                {
                    // Left Button Pressed
                    edittimedate.minute = temptimedate.minute;
                    edit_mode = HOUR;
                }

                if(butt == BIT1)
                {
                    // Right Button Pressed
                    edittimedate.minute = temptimedate.minute;
                    edit_mode = SEC;
                }
                break;
            }

            case SEC:
            {
                unsigned char pot = get_pot_scaled(1, 60);

                if(temptimedate.second != pot)
                {
                    temptimedate.second = pot;
                    displayTime(temptimedate, edit_mode); // Highlight Second
                }

                if(butt == BIT0)
                {
                    // Left Button Pressed
                    edittimedate.second = temptimedate.second;
                    edit_mode = MIN;
                }

                if(butt == BIT1)
                {
                    // Right Button Pressed
                    edittimedate.second = temptimedate.second;
                    edit_mode = MONTH;
                }
                break;
            }
            }
            break;
        }
        }

    }


}

