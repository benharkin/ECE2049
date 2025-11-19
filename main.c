#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"
#include "note.h"
#include "timer.h"

#define SONG_LENGTH 3
#define COUNTDOWN_LENGTH 3

void print_str(char *str, int32_t x, int32_t y);
void clear_display(void);

typedef enum
{
    NONE, IDLE, COUNTDOWN, PLAYING, GAME_OVER, WIN
} state_t;

//void displayNotes(Note* notes){
//    // array to hold the output for the screen
//    type array[4];
//    int array_index = 0;
//    for(int i = 0; i < 4 && array_index < 4; i++){
//        Note thisNote = notes[i];
//        if(isEnd(thisnote)){
//            //return or break or something
//        }
//        // get duration -> transform this to a height
//        int height = ??;
//        // get led
//        int led = ??;
//        for(int i = height; i >= 0; i--){
//            array[array_index] = led;
//            array_index++;
//        }
//    }
//    // display array
//}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    _BIS_SR(GIE); // Global interupts enable
    setupTimer();

    initLeds();
    configDisplay();
    configKeypad();
    initButtons();

    //char song[SONG_LENGTH];
    char song[] = { E0 | HN, D0 | HN, C0 | HN };

    state_t state = IDLE;
    state_t prev_state = NONE;

    unsigned long countdown_start;
    int countdown_elapsed = 0;
    int countdown_elapsed_prev = 0;

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
            unsigned long timerview = getTime();
            char countdown_str[] = { '\0', '\0' };
            //Countdown 3 2 1 and then move to playing state
            if (state != prev_state)
            {
                countdown_start = getTime();
                countdown_elapsed = 0;
                countdown_elapsed_prev = 0;
                countdown_str[0] = COUNTDOWN_LENGTH + 48;
                clear_display();
                print_str(countdown_str, 48, 48);
                prev_state = state;
            }

            countdown_elapsed = (getTime() - countdown_start) / 1000; // Seconds
            char countdown = COUNTDOWN_LENGTH - countdown_elapsed;

            if (countdown == 0)
            {
                state = PLAYING;
                clear_display();
                break;
            }

            if (countdown_elapsed != countdown_elapsed_prev)
            {
                countdown_str[0] = countdown + 48;
                clear_display();
                print_str(countdown_str, 48, 48);
            }

            countdown_elapsed_prev = countdown_elapsed;
//            prev_state = COUNTDOWN;
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
                BuzzerOn(song[current_note_index]);
                setLeds(getLED(song[current_note_index]));
                note_end = getTime() + getDuration(song[current_note_index]);
                stored_input = 0;
                prev_state = state;
            }
            //displayNotes(song + current_note_index)
            if (getTime() < note_end)
            {
                //note is still playing
                //check if button input matches the note
                stored_input |= button;
            }
            else
            {
                BuzzerOff();
                setLeds(0);

                if (stored_input != getLED(song[current_note_index]))
                {
                    //Wrong note, game over
                    state = GAME_OVER;
                    break;
                }

                if (current_note_index == SONG_LENGTH - 1)
                {
                    state = WIN;
                    break;
                }

                current_note_index++;
                BuzzerOn(song[current_note_index]);
                setLeds(getLED(song[current_note_index]));
                note_end = getTime() + getDuration(song[current_note_index]);
                stored_input = 0;

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

void print_str(char *str, int32_t x, int32_t y)
{
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                x, y,
                                TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void clear_display(void)
{
    Graphics_clearDisplay(&g_sContext);
    Graphics_flushBuffer(&g_sContext);
}
