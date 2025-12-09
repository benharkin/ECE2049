#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"
#include "timer.h"
#include "display.h"
#include "sensor.h"

typedef enum
{
    EDIT, DATE, TIME, TEMP_C, TEMP_F, NONE
} state_t;

typedef enum
{
    MONTH, DAY, HOUR, MIN, SEC, NONE
} edit_t;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    _BIS_SR(GIE); // Global interupts enable
    setupTimer();
    setupADC();


    configDisplay();
    configKeypad();
    initButtons();

    state_t state = DATE;
    state_t prev_state = NONE;
    edit_t edit_mode = MONTH;
    edit_t prev_edit = NONE;

    char key;
    char prevKey = 0;
    short int count = 0;
    unsigned long prev_time = 0;

    //Initial time
    timedate_t init_timedate = {6,27,0,0,0};
    setTime(init_timedate);

    unsigned long time;
    timedate_t timedate;
    timedate_t temptimedate;



    while (1)
    {

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
                state = DATE;
            else
            {
                state = EDIT;
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
                displayDate(timedate);
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
                displayTime(timedate);
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
            switch(edit_mode)
            {
            case MONTH:
            {
                // Get month and day
                // Set temporary month to potentiometer
                // Don't update display if potentiometer reads the same as previous?
                // Displays the Month and Day (Highlight Month)
                // Check left/right Button Pressed
                    // If so, set new date and time to previous
                    // date and time with updated month and temptimedate variable
                    // Switch edit_mode to next/previous case

            }

            case DAY:
            {
                //
            }

            case HOUR:
            {
                //
            }

            case MIN:
            {
                //
            }

            case SEC:
            {
                //
            }


            }

            break;
        }
        }
        prevKey = key;
        prev_time = time;
    }


}

