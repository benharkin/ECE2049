#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"
#include "note.h"
#include "timer.h"
#include "display.h"
#include "songs.h"

#define COUNTDOWN_LENGTH 3
#define NOTE_SPACING 1 // Number of ticks of silence as notes start and end.

typedef enum
{
    NONE, IDLE, COUNTDOWN, PLAYING, GAME_OVER, WIN
} state_t;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    _BIS_SR(GIE); // Global interupts enable
    setupTimer();

    initLeds();
    configDisplay();
    configKeypad();
    initButtons();

    char *song = twinkle;

    state_t state = IDLE;
    state_t prev_state = NONE;

    char key;
    char prevKey = 0;
    int count = 0;

    while (1)
    {

        key = getKey();

        // # key resets the game to IDLE
        if (key != prevKey && key == '#')
        {
            state = IDLE;
        }

        switch (state)
        {
        case IDLE:
        {
            //Display welcome screen, wait for *
            if (state != prev_state)
            {
                BuzzerOff();
                setLeds(0);
                clear_display();
                print_str("Welcome to", 48, 43);
                print_str("Guitar Hero", 48, 53);
                show_print();
                prev_state = state;
            }

            if (key != prevKey && key == '*')
            {
                state = COUNTDOWN;
            }
            break;
        }
        case COUNTDOWN:
        {
            unsigned int prevSeconds;
            int countdown;
            if (state != prev_state)
            {
                prevSeconds = 0; // So it will always not equal the current time on transition.
                countdown = COUNTDOWN_LENGTH;
                prev_state = state;
            }

            if (countdown == 0)
            {
                state = PLAYING;
                break;
            }

            if (getSeconds() != prevSeconds)
            {
                // Print the countdown
                char countdown_str[] = { countdown + 48, '\0' };
                clear_display();
                print_str(countdown_str, 48, 48);
                show_print();

                // Decrement and store time to wait 1s until next action.
                countdown--;
                prevSeconds = getSeconds();
            }
            break;
        }
        case PLAYING:
        {
            //
            //Loop through the song array
            //On each loop
            unsigned int note_index;
            unsigned long prev_time;
            unsigned long note_start;
            unsigned long note_end;
            unsigned char input;
            unsigned int display_skip_rows;
            Note this_note;

            unsigned long now = getTime();

            if (state != prev_state)
            {
                input = 0;
                note_index = 0;
                this_note = song[note_index];
                note_start = now;
                note_end = note_start + getDuration(this_note);
                display_skip_rows = 0;
                prev_time = 0;
                prev_state = state;
            }

            input |= getButtons();
            // Any of the other bits are 1 -> any wrong key was pressed
            int wrong_note = (input & ~getLED(this_note)) != 0;
            // The correct bit is 1 -> correct key was pressed
            int correct_note = (input & getLED(this_note)) != 0;

            // Wrong input at any time or no correct input by the end
            if (wrong_note || (now > note_end && !correct_note))
            {
                setLeds(0);
                BuzzerOff();
                state = GAME_OVER;
                break;
            }

            // Prev note has ended
            if (now > note_end)
            {
                note_index++;
                this_note = song[note_index];
                note_start = now;
                note_end = note_start + getDuration(this_note);
                displayNotes(song, note_index, 0);
                display_skip_rows = 0;
                input = 0;
                count = 0;
            }

            // Reached end of the song
            if (isEnd(this_note))
            {
                setLeds(0);
                BuzzerOff();
                state = WIN;
                break;
            }

            // Begin playing the note after a delay
            if (now >= note_start + NOTE_SPACING && now != prev_time)
            {
                if (!isRest(this_note))
                {
                    BuzzerOn(getPeriod(this_note));
                    setLeds(getLED(this_note));
                }
            }

            if((now - note_start) / (WHOLE_NOTE/4) == count + 1)
            {
                count++;
            }

            // Stop playing the note with a buffer before the true end
            if (now >= note_end - NOTE_SPACING && now != prev_time)
            {
                BuzzerOff();
                setLeds(0);
            }



//            if ((now - note_start) / (WHOLE_NOTE / 8)
//                    != (prev_time - note_start) / (WHOLE_NOTE / 8))
//            {
//                display_skip_rows++;
//                DisplayRow rows[8];
//                calcRows(song + note_index, rows, 8, display_skip_rows);
//                drawRowsDirect(rows, 8);
//            }

            prev_time = now;

            break;
        }
        case GAME_OVER:
        {
            //Display game over and after 1 second go to idle
            //Play a losing jingle
            if (state != prev_state)
            {
                clear_display();
                print_str("Game Over!", 48, 48);
                show_print();
                prev_state = state;
            }
            break;
        }
        case WIN:
        {
            if (state != prev_state)
            {
                clear_display();
                print_str("You Win!", 48, 48);
                show_print();
                //Display win and after 1 second go to idle
                prev_state = state;
            }
            break;
        }
        }

        prevKey = key;
    }

}

