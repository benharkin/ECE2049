#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"
#include "note.h"
#include "timer.h"
#include "display.h"

#define COUNTDOWN_LENGTH 3
#define NOTE_SPACING 2 // Number of ticks of silence as notes start and end.

typedef enum
{
    NONE, IDLE, COUNTDOWN, PLAYING, GAME_OVER, WIN
} state_t;

char twinkle[] = {
C0 | HN,
                   C0 | HN, G0 | HN, G0 | HN,
                   A1 | HN,
                   A1 | HN, G0 | WN,

                   F0 | HN,
                   F0 | HN, E0 | HN, E0 | HN,
                   D0 | HN,
                   D0 | HN, C0 | WN,

                   G0 | HN,
                   G0 | HN, F0 | HN, F0 | HN,
                   E0 | HN,
                   E0 | HN, D0 | WN,

                   G0 | HN,
                   G0 | HN, F0 | HN, F0 | HN,
                   E0 | HN,
                   E0 | HN, D0 | WN,

                   C0 | HN,
                   C0 | HN, G0 | HN, G0 | HN,
                   A1 | HN,
                   A1 | HN, G0 | WN, 0 };

//char twinkle[] = {
//C0 | HN,
//                   REST | QN, C0 | HN, REST | QN, G0 | HN, REST | QN, G0 | HN,
//                   REST | QN,
//                   A1 | HN,
//                   REST | QN,
//                   A1 | HN, REST | QN, G0 | WN, REST | QN,
//
//                   F0 | HN,
//                   REST | QN, F0 | HN, REST | QN, E0 | HN, REST | QN, E0 | HN,
//                   REST | QN,
//                   D0 | HN,
//                   REST | QN,
//                   D0 | HN, REST | QN, C0 | WN, REST | QN,
//
//                   G0 | HN,
//                   REST | QN, G0 | HN, REST | QN, F0 | HN, REST | QN, F0 | HN,
//                   REST | QN,
//                   E0 | HN,
//                   REST | QN,
//                   E0 | HN, REST | QN, D0 | WN, REST | QN,
//
//                   G0 | HN,
//                   REST | QN, G0 | HN, REST | QN, F0 | HN, REST | QN, F0 | HN,
//                   REST | QN,
//                   E0 | HN,
//                   REST | QN,
//                   E0 | HN, REST | QN, D0 | WN, REST | QN,
//
//                   C0 | HN,
//                   REST | QN, C0 | HN, REST | QN, G0 | HN, REST | QN, G0 | HN,
//                   REST | QN,
//                   A1 | HN,
//                   REST | QN,
//                   A1 | HN, REST | QN, G0 | WN, REST | QN,
//
//                   0 };

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
            unsigned char input = 0;
            Note this_note;

            unsigned long now = getTime();

            if (state != prev_state)
            {
                note_index = 0;
                prev_time = 0;
                note_start = now;
                this_note = song[note_index];
                prev_state = state;
            }

            unsigned long note_end = note_start + getDuration(this_note);

            // Prev note has ended
            if (now > note_end)
            {
                note_index++;
                this_note = song[note_index];
                note_start = now;
                input = 0;
            }

            // Reached end of the song
            if (isEnd(this_note))
            {
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

            // Stop playing the note with a buffer before the true end
            if (now >= note_end - NOTE_SPACING && now != prev_time)
            {
                BuzzerOff();
                setLeds(0);
            }

            input |= getButtons();
            // Any of the other bits are 1 -> any wrong key was pressed
            int wrong_note = (input & ~getLED(this_note)) != 0;
            // The correct bit is 1 -> correct key was pressed
            int correct_note = (input & getLED(this_note)) != 0;

            // Wrong input at any time or no correct input by the end
            if (wrong_note || (now > note_end && !correct_note))
            {
                state = GAME_OVER;
                break;
            }

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

