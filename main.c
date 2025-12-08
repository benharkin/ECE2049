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

    char key;
    char prevKey = 0;
    short int count = 0;
    unsigned long prev_time = 0;

    //Initial time
    timedate_t init_timedate = {6,27,0,0,0};
    setTime(init_timedate);

    unsigned long time;
    timedate_t timedate;



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

        // # key resets the game to IDLE
        if (key != prevKey && key == '#')
        {
            if(state == EDIT)
                state = DATE;
            else
                state = EDIT;
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
//            if (state != prev_state)
//            {
//                clear_display();
//                print_str("You Win!", 48, 48);
//                show_print();
//                //Display win and after 1 second go to idle
//                prev_state = state;
//            }
            break;
        }
        }
        prevKey = key;
        prev_time = time;
    }


}

