#pragma once
#include <stdint.h>

#define BWIDTH 10
#define BHEIGHT 20

extern uint16_t board[BHEIGHT][BWIDTH];

void board_init(void);
void board_draw(void);