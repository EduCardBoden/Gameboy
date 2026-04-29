#include "board.h"
#include <avr/io.h>
#include "../../drivers/ili9341.h"

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

            ili9341_fill_rect(
                BOARD_OFFSET_X + c * TILESIZE, //x in pixel, offset damit board zentriert ist
                BOARD_OFFSET_Y + r * TILESIZE, //y in pixel
                TILESIZE, TILESIZE, color       //tile zeichnen
            );
        }
    }  
}

uint8_t board_is_empty(int8_t col, int8_t row) {
    
    if (col < 0 || row < 0 || col >= BWIDTH || row >= BHEIGHT) {

        return 0; //außerhalb des boards = wand = nicht leer
    }

    if (board[row][col] == 0) {
        
        return 1; //zelle ist leer
    }
    return 0; //zelle hat farbe = nicht leer
}

void board_lock(uint8_t col, uint8_t row, uint16_t color) {

    if (col < BWIDTH && row < BHEIGHT) {
        
        board[row][col] = color; //farbe ins array schreiben = dauerhaft besetzt
    }
}

uint8_t board_clear_lines(void) {

    uint8_t cleared = 0;

    for (uint8_t r = BHEIGHT - 1; r > 0; r--) { //von unten nach oben durchgehen

        uint8_t full = 1;

        for (uint8_t c = 0; c < BWIDTH; c++) {
            
            if (board[r][c] == 0) { 
                full = 0; //leere zelle gefunden = reihe nicht voll
                break;
            }
        }

        if (full) {

            for (uint8_t y = r; y > 0; y--) { //alle reihen darüber eine nach unten schieben

                for (uint8_t x = 0; x < BWIDTH; x++) {

                    board[y][x] = board[y-1][x];
                }
            }

            for (uint8_t x = 0; x < BWIDTH; x++) {

                board[0][x] = 0; //oberste reihe leeren
            }

            r++;        //selbe reihe nochmal checken weil neue reihe nachgerutscht ist
            cleared++;  //anzahl gelöschter reihen zählen
        }
    }
    return cleared; //zurückgeben für score berechnung
}