#pragma once
#include <stdint.h>

extern const uint8_t font[][7];

void draw_char(uint8_t c, uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t scale);
void draw_string(const char *str, uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t scale);