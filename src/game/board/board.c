#include "board.h"
#include <avr/io.h>
#include "drivers/ili9341.h"

uint16_t board[BHEIGHT][BWIDTH]; //board array, 0 = leer

void board_init(void) {

    for(uint8_t r = 0; r < BHEIGHT; r++) {

        for(uint8_t c = 0; c < BWIDTH; c++) {

            board[r][c] = 0; //alle tiles leer setzen
        }
    }
}

void board_draw(void) {

    for (uint8_t r = 0; r < BHEIGHT; r++) {

        for(uint8_t c = 0; c < BWIDTH; c++) {

            uint16_t color;

            if (board[r][c] == 0) {

                color = COLOR_BLACK; //leer = schwarz
            } 
            
            else {

                color = board[r][c]; //gespeicherte farbe nehmen
            }

            ili9341_fill_rect(c * TILESIZE, r * TILESIZE, TILESIZE, TILESIZE, color); //tile zeichnen
        }
    }  
    
}

uint8_t board_is_empty(uint8_t col, uint8_t row) {
    
    if (col >= BWIDTH || row >= BHEIGHT) {

        return 0; //außerhalb des boards = wie eine wand = nicht leer
    }

    if (board[row][col] == 0) {
        
        return 1; //zelle ist leer
    }
    return 0; //zelle hat eine farbe = nicht leer
}

void board_lock(uint8_t col, uint8_t row, uint16_t color) {

    if (col < BWIDTH && row < BHEIGHT) {
        
        board[row][col] = color; //farbe in den board array schreiben
    }
}

uint8_t board_clear_lines(void) {

    uint8_t cleared = 0;

    for (uint8_t r = 0; r < BHEIGHT; r++) {

        uint8_t full = 1;

        for (uint8_t c = 0; c < BWIDTH; c++) {

            if (board[r][c] == 0) { 

                full = 0; //eine leere zelle gefunden = reihe nicht voll
                break;
            }
        }
        if (full) {

            for (uint8_t y = r; y > 0; y--) { //alles eine reihe nach unten schieben

                for (uint8_t x = 0; x < BWIDTH; x++) {

                    board[y][x] = board[y-1][x];
                }
            }
            for (uint8_t x = 0; x < BWIDTH; x++) {

                board[0][x] = 0; //oberste reihe leeren
            }
            cleared++;
        }
    }
    return cleared; //wie viele reihen wurden gecleart
}
