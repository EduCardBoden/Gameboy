#ifndef ILI9341_H
#define ILI9341_H

#include <stdint.h>

#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 320

//Colors
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW  0xFFE0

#define RGB565(r, g, b) (((uint16_t)(r & 0x1F) << 11) | ((uint16_t)(g & 0x3F) << 5) | (b & 0x1F))

void ili9341_init(void);
void ili9341_fill_screen(uint16_t color);
void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

#endif