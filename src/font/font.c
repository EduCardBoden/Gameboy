#include "font.h"
#include <avr/io.h>
#include "drivers/ili9341.h"

void draw_char(uint8_t c, uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t scale) {

    for(int row = 0; row < 7; row++) { //alle 7 zeilen durchgehen

        for(int col = 0; col < 5; col++) { //alle 5 spalten durchgehen

            if(font[c - '0'][row] & (0x80 >> col)) { //gibt es ein pit in pos col?

                ili9341_fill_rect(x + col * scale, y + row * scale, scale, scale, color); //ja -> vordergrund farbe
            }
            else {

                ili9341_fill_rect(x + col * scale, y + row * scale, scale, scale, bg); //nein -> hintergrund farbe
            }
        }
    }
}

void draw_string(const char *str, uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t scale) {
    while (*str) { //solange kein \0 (string ende)    
                        
        draw_char(*str, x, y, color, bg, scale); //aktuelle char zeichnen
        str++; //pointer zu nächsten char
        x += 6 * scale; //6 pixel weiter nach rechts
    }
}