#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"
#include "note.h"
#include "timer.h"
#include "display.h"

#define COUNTDOWN_LENGTH 3

typedef enum
{
    NONE, IDLE, COUNTDOWN, PLAYING, GAME_OVER, WIN
} state_t;
/*
 char twinkle[] = {
 C0 | HN, REST | QN ,C0 | HN, REST | QN, G0 | HN, REST | QN, G0 | HN,
 A1 | HN, A1 | HN, G0 | WN,

 F0 | HN, F0 | HN, E0 | HN, E0 | HN,
 D0 | HN, D0 | HN, C0 | WN,

 G0 | HN, G0 | HN, F0 | HN, F0 | HN,
 E0 | HN, E0 | HN, D0 | WN,

 G0 | HN, G0 | HN, F0 | HN, F0 | HN,
 E0 | HN, E0 | HN, D0 | WN,

 C0 | HN, C0 | HN, G0 | HN, G0 | HN,
 A1 | HN, A1 | HN, G0 | WN, 0
 };
 */
char twinkle[] = {
C0 | HN,
                   REST | QN, C0 | HN, REST | QN, G0 | HN, REST | QN, G0 | HN,
                   REST | QN,
                   A1 | HN,
                   REST | QN, A1 | HN, REST | QN, G0 | WN, REST | QN,

                   F0 | HN,
                   REST | QN, F0 | HN, REST | QN, E0 | HN, REST | QN, E0 | HN,
                   REST | QN,
                   D0 | HN,
                   REST | QN, D0 | HN, REST | QN, C0 | WN, REST | QN,

                   G0 | HN,
                   REST | QN, G0 | HN, REST | QN, F0 | HN, REST | QN, F0 | HN,
                   REST | QN,
                   E0 | HN,
                   REST | QN, E0 | HN, REST | QN, D0 | WN, REST | QN,

                   G0 | HN,
                   REST | QN, G0 | HN, REST | QN, F0 | HN, REST | QN, F0 | HN,
                   REST | QN,
                   E0 | HN,
                   REST | QN, E0 | HN, REST | QN, D0 | WN, REST | QN,

                   C0 | HN,
                   REST | QN, C0 | HN, REST | QN, G0 | HN, REST | QN, G0 | HN,
                   REST | QN,
                   A1 | HN,
                   REST | QN, A1 | HN, REST | QN, G0 | WN, REST | QN,

                   0 };

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

    unsigned long note_end;

    int current_note_index = 0;

    char key;
    char prevKey;

    char button;
    char prev_button;

    char stored_input;

    while (1)
    {

        key = getKey();
        button = getButtons();

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

            if(countdown == 0){
                state = PLAYING;
                break;
            }

            if(getSeconds() != prevSeconds){
                // Print the countdown
                char countdown_str[] = {countdown + 48, '\0' };
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
            if (state != prev_state)
            {
                current_note_index = 0;
                BuzzerOn(getPeriod(song[current_note_index]));
                setLeds(getLED(song[current_note_index]));
                note_end = getTime() + getDuration(song[current_note_index]);
                //quarter_end =  getTime() + WHOLE_NOTE/4;
                displayNotes(song + current_note_index);
                stored_input = 0;
                prev_state = state;
            }

            if (getTime() < note_end)
            {
                //note is still playing
                //check if button input matches the note
                stored_input |= button;
                //if(getTime() < quarter_end)
                //    displayNotes(song + current_note_index);

            }
            else
            {
                Note current_note = song[current_note_index];
                BuzzerOff();
                setLeds(0);

                if (stored_input != getLED(current_note))
                {
                    //Wrong note, game over
                    state = GAME_OVER;
                    break;
                }

                if (isEnd(current_note))
                {
                    state = WIN;
                    break;
                }

                current_note_index++;
                current_note = song[current_note_index];
                if (!isRest(current_note))
                {
                    BuzzerOn(getPeriod(current_note));
                }

                setLeds(getLED(current_note));
                note_end = getTime() + getDuration(current_note);
                stored_input = 0;
                displayNotes(song + current_note_index);

                //play note at current_note index (buzzer and corresponding LED)
                //set note_end to timer + note duration
            }
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
        prev_button = button;
    }

}

