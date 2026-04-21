#pragma once 
#include <stdint.h>
#include "../../drivers/ili9341.h"

#define NUM_PIECES 7

static const uint16_t piece_colors[NUM_PIECES] = {

    COLOR_CYAN,     //0 - I
    COLOR_YELLOW,   //1 - O
    COLOR_MAGENTA,  //2 - T (purple approx)
    COLOR_GREEN,    //3 - S
    COLOR_RED,      //4 - Z
    COLOR_BLUE,     //5 - J
    COLOR_ORANGE,   //6 - L 
};

extern const uint8_t tetrominos[NUM_PIECES][4][4][4]; //anzahl, orientierung, reihe, spalte

void draw_tetr(uint8_t piece, uint8_t rotation, uint8_t col, uint8_t row, uint16_t color); //was, wie, wo, welche farbe
void erase_tetr(uint8_t piece, uint8_t rotation, uint8_t col, uint8_t row); //delete piece before moving into new position