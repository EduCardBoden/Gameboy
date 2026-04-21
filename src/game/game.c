#include "game.h"
#include <avr/io.h>
#include "pieces/tetrominoes.h"
#include "board/board.h"
#include "../inputs/button.h"
#include "../drivers/ili9341.h"
#include <stdlib.h>

static ActivePiece current; //globale variable

static uint8_t collision_check(uint8_t piece, uint8_t rotation, uint8_t col, uint8_t row) {

    for (uint8_t r = 0; r < 4; r++) { //jede zelle durchgehen (row)

        for (uint8_t c = 0; c < 4; c++) { //jede zelle durchgehen (col)

            if (tetrominos[piece][rotation][r][c]) { //ist diese zelle gefüllt?

                if (!board_is_empty(col + c, row + r)) { //ist die position frei?

                    return 0; //kollision
                }
            }
        }
    }
    return 1; //alles frei
}

static void spawn_piece(void) {

    current.piece    = rand() % NUM_PIECES; //zufälliges piece
    current.rotation = 0;                   //orientierung immer 0 bei start
    current.col      = 3;                   //startet in der mitte
    current.row      = 0;                   //ganz oben
}

static void lock_piece(void) {

    uint16_t color = piece_colors[current.piece]; //farbe vom aktuellen piece

    for (uint8_t r = 0; r < 4; r++) { //jede row durchgehen

        for (uint8_t c = 0; c < 4; c++) { //jede col durchgehen

            if (tetrominos[current.piece][current.rotation][r][c]) { //ist da was?

                board_lock(current.col + c, current.row + r, color); //board_lock callen
            }
        }
    }
}

static void draw_piece(void) {

    uint16_t color = piece_colors[current.piece];

    for (uint8_t r = 0; r < 4; r++) { //jede zelle durch

        for (uint8_t c = 0; c < 4; c++) { //jede zelle durch

            if (tetrominos[current.piece][current.rotation][r][c]) { //gibts was?

                ili9341_fill_rect(
                    TILE_TO_X(current.col + c), //x in pixel
                    TILE_TO_Y(current.row + r), //y in pixel
                    TILESIZE, TILESIZE, color
                );
            }
        }
    }
}

static void erase_piece(void) {

    for (uint8_t r = 0; r < 4; r++) { //jede row durch

        for (uint8_t c = 0; c < 4; c++) { //jede col durch

            if (tetrominos[current.piece][current.rotation][r][c]) { //gibts was?

                ili9341_fill_rect(
                    TILE_TO_X(current.col + c), //x in pixel
                    TILE_TO_Y(current.row + r), //y in pixel
                    TILESIZE, TILESIZE, COLOR_BLACK //schwarz = löschen
                );
            }
        }
    }
}

void game_tick(void) {

    if (collision_check(current.piece, current.rotation, current.col, current.row + 1)) { //kann piece fallen?

        erase_piece();
        current.row++; //reihe tiefer
        draw_piece();

    } else { //kann nicht fallen -> locken

        lock_piece();
        board_clear_lines();
        board_draw();
        spawn_piece();
        draw_piece();
    }
}

void game_loop(void) {

    //links bewegen
    if (button_was_pressed(BTN_LEFT)) {

        if (current.col > 0 && //underflow schutz
            collision_check(current.piece, current.rotation, current.col - 1, current.row)) {

            erase_piece();
            current.col--;
            draw_piece();
        }
    }

    //rechts bewegen
    if (button_was_pressed(BTN_RIGHT)) {

        if (collision_check(current.piece, current.rotation, current.col + 1, current.row)) {

            erase_piece();
            current.col++;
            draw_piece();
        }
    }

    //rotieren
    if (button_was_pressed(BTN_ROT)) {

        uint8_t new_rot = (current.rotation + 1) % 4; //0→1→2→3→0
        if (collision_check(current.piece, new_rot, current.col, current.row)) {

            erase_piece();
            current.rotation = new_rot;
            draw_piece();
        }
    }

    //soft drop
    if (button_was_pressed(BTN_DOWN)) {

        if (collision_check(current.piece, current.rotation, current.col, current.row + 1)) {

            erase_piece();
            current.row++;
            draw_piece();
        }
    }
}

void game_init(void) {

    board_init();                     //board leeren
    ili9341_fill_screen(COLOR_BLACK); //screen schwarz
    srand(42);                        //zufallszahlen initialisieren
    spawn_piece();                    //erstes piece spawnen
    draw_piece();                     //erstes piece zeichnen
}