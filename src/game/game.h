#pragma once
#include <stdint.h>

typedef struct {

    uint8_t piece;
    uint8_t rotation;
    int8_t col;
    int8_t row;
} ActivePiece;
typedef enum {

    GAME_MENU = 0,
    GAME_PLAY = 1,
    GAME_OVER = 2
} GameState;

extern GameState game_state;

void game_init(void);
void game_loop(void);
void game_start(void);
void game_tick(void); //von timer interrupt aufgerufen schaut alle 500ms nach ob was sich bewegen kann