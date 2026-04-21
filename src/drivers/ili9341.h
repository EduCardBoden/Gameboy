#pragma once

#include <stdint.h>

//define a constant for everywhere in the code
#define ILI9341_WIDTH 240 //display width in pixels
#define ILI9341_HEIGHT 320 // display height in pixels

//define colors in RGB565 format
#define COLOR_BLACK   0x0000 
#define COLOR_WHITE   0xFFFF 
#define COLOR_RED     0xF800 
#define COLOR_GREEN   0x07E0 
#define COLOR_BLUE    0x001F 
#define COLOR_CYAN    0x07FF 
#define COLOR_MAGENTA 0xF81F 
#define COLOR_YELLOW  0xFFE0 
#define COLOR_ORANGE  0xFAA0

#define RGB565(r, g, b) (((uint16_t)(r & 0x1F) << 11) | ((uint16_t)(g & 0x3F) << 5) | (b & 0x1F)) //macro (#define that takes input like functions)

//declaring functions for other files
void ili9341_init(void); //starts configurations and communications
void ili9341_fill_screen(uint16_t color); //fills the screen with a color
void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color); //fills in a rectangle at x,y with specifications
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color); //draws a pixel in x,y with specific color

