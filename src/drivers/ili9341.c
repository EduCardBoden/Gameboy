#define F_CPU 16000000UL
#include "ili9341.h"
#include "spi.h"
#include <avr/io.h>
#include <util/delay.h>

// pin definitions (andere schreibweise geben)
#define DC_DDR   DDRD
#define DC_PORT  PORTD
#define DC_PIN   PD7
#define RST_DDR  DDRD
#define RST_PORT PORTD
#define RST_PIN  PD6
#define CS_DDR   DDRB
#define CS_PORT  PORTB
#define CS_PIN   PB2

//pin macros (leichter zum sehen was im code passiert)
#define CS_LOW()   (CS_PORT &= ~(1 << CS_PIN))
#define CS_HIGH()  (CS_PORT |=  (1 << CS_PIN))
#define DC_CMD()   (DC_PORT &= ~(1 << DC_PIN))
#define DC_DATA()  (DC_PORT |=  (1 << DC_PIN))
#define RST_LOW()  (RST_PORT &= ~(1 << RST_PIN))
#define RST_HIGH() (RST_PORT |=  (1 << RST_PIN))

//static helpers 
static void ili_cmd(uint8_t cmd) {
    DC_CMD(); //DC low pullen = command kommt gleich
    CS_LOW(); //CS low pullen = display soll zuhören was kommt
    spi_transfer(cmd); //schicke den command byte
    CS_HIGH(); //release CS = fertig mit kommunikation
}

//das selbe wie das obere aber statt command kommt data
static void ili_data8(uint8_t data) {
    DC_DATA(); //DC high = data kommt gleich
    CS_LOW();
    spi_transfer(data);
    CS_HIGH();
}

//selbe wie oben aber für 16 bit values
static void ili_data16(uint16_t data) {
    DC_DATA();
    CS_LOW();
    spi_transfer(data >> 8); //spi kann nur 8 bits aufeinmal also in 2 bytes splitten
    spi_transfer(data & 0xFF);
    CS_HIGH();
}

static void ili_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ili_cmd(0x2A); //setzt column range
    ili_data16(x0); //linke ecke
    ili_data16(x1); //rechte ecke 
    ili_cmd(0x2B); //setzt row range
    ili_data16(y0); //obere ecke
    ili_data16(y1); //untere ecke
    ili_cmd(0x2C); //memory write - pixel data kommt gleich
}

//public functions
void ili9341_init(void) {
    DC_DDR  |= (1 << DC_PIN); //DC output
    RST_DDR |= (1 << RST_PIN); //RST output
    CS_DDR  |= (1 << CS_PIN); //CS output
    CS_HIGH(); //high bedeutet nicht ausgewählt (sonst hört er zu wenn man es nicht braucht)

    //hardware reset, zeiten kommen vom ILI9341 datenblatt
    RST_HIGH(); _delay_ms(5); //zuerst auf high damit mann mit einem known state arbeitet
    RST_LOW();  _delay_ms(20); 
    RST_HIGH(); _delay_ms(150); //display braucht 150ms vor er commands nehmen kann

    ili_cmd(0x01); _delay_ms(5); //software reset
    ili_cmd(0x11); _delay_ms(120); //aufwecken von sleep mode
    ili_cmd(0x3A); ili_data8(0x55); //erwarte pixel format RGB565
    ili_cmd(0x36); ili_data8(0x48); //setz orientierung und RBG farbreihe
    ili_cmd(0x29); _delay_ms(20); //display an
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return; //alles muss innerhalb des displays bleiben -safety check
    ili_set_window(x, y, x, y); //nur ein pixel zeichnen bei x,y 
    ili_data16(color); //16 bit farbwert
}

void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return; //alles muss innerhalb des displays bleiben -safety check
    if (x + w > ILI9341_WIDTH)  w = ILI9341_WIDTH  - x; //shrink rechteck falls es außerhalb der grenze geht
    if (y + h > ILI9341_HEIGHT) h = ILI9341_HEIGHT - y; //shrink rechteck falls es außerhalb der grenze geht

    ili_set_window(x, y, x + w - 1, y + h - 1); //-1 weil set_window letzte pixel koordinate braucht, nicht breite
    DC_DATA();
    CS_LOW(); //CS bleibt low für die ganze schleife 
    uint32_t total = (uint32_t)w * h; //32 bit weil 16 zu klein ist
    uint8_t hi = color >> 8;  //farbe einmal splitten vor der schleife (spart rechenzeit)
    uint8_t lo = color & 0xFF; 
    for (uint32_t i = 0; i < total; i++) { // jeden pixel mit hi und lo byte befüllen
        spi_transfer(hi); 
        spi_transfer(lo);
    }
    CS_HIGH(); //CS freisetzen weil schleife vorbei ist
}

void ili9341_fill_screen(uint16_t color) {
    ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color); //fill_rect aber für den ganzen screen
}