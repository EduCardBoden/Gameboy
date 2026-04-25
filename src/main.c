#define F_CPU 16000000UL
#include "game/board/board.h"
#include "game/game.h"
#include "game/timer/timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "drivers/spi.h"
#include "drivers/ili9341.h"
#include "inputs/button.h"
#include "font/font.h"
#include "buzzer/buzzer.h"
#include <util/delay.h>
#include "game/board/board.h"
#include "game/pieces/tetrominoes.h"
#include "game/game.h"
#include <avr/interrupt.h>
#include "game/menu/menu.h"

static const BuzzerNote tetris[] = {
    //A teil
    {NOTE_E5, DUR_QUARTER}, {NOTE_B4, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_D5, DUR_QUARTER}, {NOTE_C5, DUR_EIGHTH},  {NOTE_B4, DUR_EIGHTH},
    {NOTE_A4, DUR_QUARTER}, {NOTE_A4, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_E5, DUR_QUARTER}, {NOTE_D5, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_B4, DUR_DOT_QUARTER}, {NOTE_C5, DUR_EIGHTH}, {NOTE_D5, DUR_QUARTER},
    {NOTE_E5, DUR_QUARTER}, {NOTE_C5, DUR_QUARTER}, {NOTE_A4, DUR_QUARTER},
    {NOTE_A4, DUR_HALF},    {NOTE_REST, DUR_QUARTER},

    {NOTE_D5, DUR_QUARTER}, {NOTE_F5, DUR_EIGHTH},  {NOTE_A5, DUR_QUARTER},
    {NOTE_G5, DUR_EIGHTH},  {NOTE_F5, DUR_EIGHTH},  {NOTE_E5, DUR_DOT_QUARTER},
    {NOTE_C5, DUR_EIGHTH},  {NOTE_E5, DUR_QUARTER},  {NOTE_D5, DUR_EIGHTH},
    {NOTE_C5, DUR_EIGHTH},  {NOTE_B4, DUR_DOT_QUARTER}, {NOTE_C5, DUR_EIGHTH},
    {NOTE_D5, DUR_QUARTER}, {NOTE_E5, DUR_QUARTER},
    {NOTE_C5, DUR_QUARTER}, {NOTE_A4, DUR_QUARTER},
    {NOTE_A4, DUR_HALF},    {NOTE_REST, DUR_QUARTER},

    //A teil wiederholung
    {NOTE_E5, DUR_QUARTER}, {NOTE_B4, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_D5, DUR_QUARTER}, {NOTE_C5, DUR_EIGHTH},  {NOTE_B4, DUR_EIGHTH},
    {NOTE_A4, DUR_QUARTER}, {NOTE_A4, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_E5, DUR_QUARTER}, {NOTE_D5, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_B4, DUR_DOT_QUARTER}, {NOTE_C5, DUR_EIGHTH}, {NOTE_D5, DUR_QUARTER},
    {NOTE_E5, DUR_QUARTER}, {NOTE_C5, DUR_QUARTER}, {NOTE_A4, DUR_QUARTER},
    {NOTE_A4, DUR_HALF},    {NOTE_REST, DUR_QUARTER},

    {NOTE_D5, DUR_QUARTER}, {NOTE_F5, DUR_EIGHTH},  {NOTE_A5, DUR_QUARTER},
    {NOTE_G5, DUR_EIGHTH},  {NOTE_F5, DUR_EIGHTH},  {NOTE_E5, DUR_DOT_QUARTER},
    {NOTE_C5, DUR_EIGHTH},  {NOTE_E5, DUR_QUARTER},  {NOTE_D5, DUR_EIGHTH},
    {NOTE_C5, DUR_EIGHTH},  {NOTE_B4, DUR_DOT_QUARTER}, {NOTE_C5, DUR_EIGHTH},
    {NOTE_D5, DUR_QUARTER}, {NOTE_E5, DUR_QUARTER},
    {NOTE_C5, DUR_QUARTER}, {NOTE_A4, DUR_QUARTER},
    {NOTE_A4, DUR_HALF},    {NOTE_REST, DUR_QUARTER},

    //B teil
    {NOTE_E5, DUR_HALF},    {NOTE_C5, DUR_HALF},
    {NOTE_D5, DUR_HALF},    {NOTE_B4, DUR_HALF},
    {NOTE_C5, DUR_HALF},    {NOTE_A4, DUR_HALF},
    {NOTE_GS4, DUR_HALF},   {NOTE_B4, DUR_QUARTER}, {NOTE_REST, DUR_QUARTER},

    {NOTE_E5, DUR_HALF},    {NOTE_C5, DUR_HALF},
    {NOTE_D5, DUR_HALF},    {NOTE_B4, DUR_HALF},
    {NOTE_C5, DUR_QUARTER}, {NOTE_E5, DUR_QUARTER}, {NOTE_A5, DUR_HALF},
    {NOTE_A5, DUR_HALF},    {NOTE_REST, DUR_QUARTER},

    //A teil final
    {NOTE_E5, DUR_QUARTER}, {NOTE_B4, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_D5, DUR_QUARTER}, {NOTE_C5, DUR_EIGHTH},  {NOTE_B4, DUR_EIGHTH},
    {NOTE_A4, DUR_QUARTER}, {NOTE_A4, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_E5, DUR_QUARTER}, {NOTE_D5, DUR_EIGHTH},  {NOTE_C5, DUR_EIGHTH},
    {NOTE_B4, DUR_DOT_QUARTER}, {NOTE_C5, DUR_EIGHTH}, {NOTE_D5, DUR_QUARTER},
    {NOTE_E5, DUR_QUARTER}, {NOTE_C5, DUR_QUARTER}, {NOTE_A4, DUR_QUARTER},
    {NOTE_A4, DUR_HALF},    {NOTE_REST, DUR_QUARTER},

    {NOTE_D5, DUR_QUARTER}, {NOTE_F5, DUR_EIGHTH},  {NOTE_A5, DUR_QUARTER},
    {NOTE_G5, DUR_EIGHTH},  {NOTE_F5, DUR_EIGHTH},  {NOTE_E5, DUR_DOT_QUARTER},
    {NOTE_C5, DUR_EIGHTH},  {NOTE_E5, DUR_QUARTER},  {NOTE_D5, DUR_EIGHTH},
    {NOTE_C5, DUR_EIGHTH},  {NOTE_B4, DUR_DOT_QUARTER}, {NOTE_C5, DUR_EIGHTH},
    {NOTE_D5, DUR_QUARTER}, {NOTE_E5, DUR_QUARTER},
    {NOTE_C5, DUR_QUARTER}, {NOTE_A4, DUR_QUARTER},
    {NOTE_A4, DUR_HALF},
};

#define tetris_length (sizeof(tetris) / sizeof(tetris[0]))  //anzahl der notes automatisch rechnen

int main(void)
{

   spi_init();
   ili9341_init();
   timer_init();
   game_init();
   button_init();
   //buzzer_init();
   menu_draw(COLOR_BLACK, "DU HOSENSOHN");
   sei(); //interrupt enable

   
   while (1)
   {

    switch(game_state) {

        case GAME_MENU: 

        if(button_was_pressed(BTN_START)) {

            game_state = GAME_PLAY;
           
            game_start();
        }
        break;

        case GAME_PLAY:
        
        game_loop();

        if(button_was_pressed(BTN_PAUSE)) {

            TIMSK2 &= ~(1 << OCIE2A);
            game_state = GAME_MENU;
            menu_draw(COLOR_BLUE, "TETRIS");
           
        }
        break;

    }
    
   
    //game_loop();
    //buzzer_play(tetris, tetris_length);
   }

   return 0;
}