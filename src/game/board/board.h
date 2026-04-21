#pragma once
#include <stdint.h>

#define BWIDTH 10
#define BHEIGHT 20
#define TILESIZE 16                                                    
#define BOARD_OFFSET_X ((ILI9341_WIDTH  - BWIDTH  * TILESIZE) / 2)     
#define BOARD_OFFSET_Y ((ILI9341_HEIGHT - BHEIGHT * TILESIZE) / 2)     
#define TILE_TO_X(col) (BOARD_OFFSET_X + (col) * TILESIZE)
#define TILE_TO_Y(row) (BOARD_OFFSET_Y + (row) * TILESIZE)

extern uint16_t board[BHEIGHT][BWIDTH];

void board_init(void);
void board_draw(void); //draw alle locked tiles
uint8_t board_is_empty(int8_t col, int8_t row); //is dieser tile frei?
void board_lock(uint8_t col, uint8_t row, uint16_t color); //lock a tile
uint8_t board_clear_lines(void);  //check und clear full rows returns how many cleared