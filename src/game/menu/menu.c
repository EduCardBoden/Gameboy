#include "menu.h"
#include "../../drivers/ili9341.h"
#include "../../font/font.h"
#include <avr/io.h>
#include <string.h>

static uint16_t center_x(const char *str, uint8_t scale) { //mitte von x position

    uint16_t text_width = strlen(str) * 6 * scale;
    return (ILI9341_WIDTH - text_width) / 2;
}

static uint16_t center_y(uint8_t scale) { //mitte von y position

    uint16_t text_height = 7 * scale;
    return (ILI9341_HEIGHT - text_height) / 2;
}

void menu_draw(uint16_t bg, const char *str) {

    ili9341_fill_screen(bg); //make the menu with the color

    uint8_t scale = 2;
    uint16_t line_height = 7 * scale;

    uint16_t title_x = center_x(str, scale);
    uint16_t title_y = center_y(scale);
    uint16_t HALLO_x =  center_x("HALLO", scale);


    draw_string(str, title_x, title_y, COLOR_WHITE, bg, scale);
    draw_string("HALLO", HALLO_x, title_y - 20, COLOR_WHITE, bg, scale);
}