#include <msp430.h>
#include "peripherals.h"
#include "buttons.h"

#define A0 0x00
#define B BIT0
#define C BIT1
#define D BIT1|BIT0
#define E BIT2
#define F BIT2|BIT0
#define G BIT2|BIT1
#define A1 BIT2|BIT1|BIT0



#define SONG_LENGTH 28

void setupTimer(void){
    //Timer A2 setup for interrupts every 5 ms
    TA2CTL = TASSEL_1 | ID_0 | MC_1;
    TACCR0 = 163;
    TA2CCTL0 = CCIE;
}

void print_str(char *str, int32_t x, int32_t y)
{
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, (uint8_t*) str, AUTO_STRING_LENGTH,
                                x, y,
                                TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

typedef enum
{
    NONE, IDLE, COUNTDOWN, PLAYING, GAME_OVER, WIN
} state_t;


volatile long timer;


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
    char prev_key;

    char button;
    char prev_button;

    char stored_input;

    while (1){

        key = getKey();
        button = getButton();

        switch (state){
        case IDLE:
            //Display welcome screen, wait for *
            if (state != prev_state)
            {
                print_str("Welcome to Guitar Hero", 48, 48);
                prev_state = state;
            }

//            wait_for_key_press('*');
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

            if(timer<note_end){
                //note is still playing
                //check if button input matches the note
                stored_input |= button;
            } else {
                buzzerOff();
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
                buzzerOn(song[current_note]);
                setLeds(song[current_note] & (BIT0|BIT1|BIT2|BIT3))
                note_end = timer + getDuration(song[current_note]);

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

        prev_key = key;
        prev_button = button;
    }

}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER_A2_ISR (void)
{
    timer++;
}



