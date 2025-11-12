#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"
#include "note.h"
#include "timer.h"

#define SONG_LENGTH 28

void print_str(char *str, int32_t x, int32_t y);

typedef enum
{
    NONE, IDLE, COUNTDOWN, PLAYING, GAME_OVER, WIN
} state_t;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer

    initLeds();
    configDisplay();
    configKeypad();
    initButtons();


    char song[SONG_LENGTH];

    state_t state = IDLE;
    state_t prev_state = NONE;

    const int countdown_start = 3;
    int countdown = countdown_start;

    int note_end;

    int current_note = 0;

    char key;
    char prevKey;

    char button;
    char prev_button;

    char stored_input;

    while (1){

        key = getKey();
        button = getButtons();

        switch (state){
        case IDLE:
            //Display welcome screen, wait for *
            if (state != prev_state)
            {
                print_str("Welcome to Guitar Hero", 48, 48);
                prev_state = state;
            }

            if (key != prevKey && key == '*')
            {
                state = COUNTDOWN;
            }
            break;
        case COUNTDOWN:
            //Countdown 3 2 1 and then move to playing state
            break;
        case PLAYING:
            //
            //Loop through the song array
            //On each loop

            if(getTime()<note_end){
                //note is still playing
                //check if button input matches the note
                stored_input |= button;
            } else {
                BuzzerOff();
                setLeds(0);

                if(stored_input != song[current_note] & (BIT0|BIT1|BIT2|BIT3)){
                    //Wrong note, game over
                    state = GAME_OVER;
                    break;
                }

                if (current_note == SONG_LENGTH - 1){
                    state = WIN;
                    break;
                }

                current_note++;
                BuzzerOn(song[current_note]);
                setLeds(song[current_note] & (BIT0|BIT1|BIT2|BIT3));
                note_end = getTime() + getDuration(song[current_note]);

                //play note at current_note index (buzzer and corresponding LED)
                //set note_end to timer + note duration
            }
            break;
        case GAME_OVER:
            //Display game over and after 1 second go to idle
            //Play a losing jingle
            break;
        case WIN:
            //Display win and after 1 second go to idle
            break;
        }

        prevKey = key;
        prev_button = button;
    }

}

void print_str(char *str, int32_t x, int32_t y)
{
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                x, y,
                                TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}
