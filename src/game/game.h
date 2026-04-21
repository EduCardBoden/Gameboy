#pragma once
#include <stdint.h>

typedef struct {

    uint8_t piece;
    uint8_t rotation;
    int8_t col;
    int8_t row;
} ActivePiece;

void game_init(void);
void game_loop(void);
void game_tick(void); //von timer interrupt aufgerufen schaut alle 500ms nach ob was sich bewegen kann