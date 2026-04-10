#include "ili9341.h"
#include "spi.h"
#include <avr/io.h>
#include <util/delay.h>

// pin definitions
#define DC_DDR   DDRD
#define DC_PORT  PORTD
#define DC_PIN   PD7
#define RST_DDR  DDRD
#define RST_PORT PORTD
#define RST_PIN  PD6
#define CS_DDR   DDRB
#define CS_PORT  PORTB
#define CS_PIN   PB2

//pin macros
#define CS_LOW()   (CS_PORT &= ~(1 << CS_PIN))
#define CS_HIGH()  (CS_PORT |=  (1 << CS_PIN))
#define DC_CMD()   (DC_PORT &= ~(1 << DC_PIN))
#define DC_DATA()  (DC_PORT |=  (1 << DC_PIN))
#define RST_LOW()  (RST_PORT &= ~(1 << RST_PIN))
#define RST_HIGH() (RST_PORT |=  (1 << RST_PIN))

//static helpers
static void ili_cmd(uint8_t cmd) {
    DC_CMD();
    CS_LOW();
    spi_transfer(cmd);
    CS_HIGH();
}

static void ili_data8(uint8_t data) {
    DC_DATA();
    CS_LOW();
    spi_transfer(data);
    CS_HIGH();
}

static void ili_data16(uint16_t data) {
    DC_DATA();
    CS_LOW();
    spi_transfer(data >> 8);
    spi_transfer(data & 0xFF);
    CS_HIGH();
}

static void ili_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ili_cmd(0x2A);
    ili_data16(x0);
    ili_data16(x1);
    ili_cmd(0x2B);
    ili_data16(y0);
    ili_data16(y1);
    ili_cmd(0x2C);
}

//public functions
void ili9341_init(void) {
    DC_DDR  |= (1 << DC_PIN);
    RST_DDR |= (1 << RST_PIN);
    CS_DDR  |= (1 << CS_PIN);
    CS_HIGH();

    RST_HIGH(); _delay_ms(5);
    RST_LOW();  _delay_ms(20);
    RST_HIGH(); _delay_ms(150);

    ili_cmd(0x01); _delay_ms(5);
    ili_cmd(0x11); _delay_ms(120);
    ili_cmd(0x3A); ili_data8(0x55);
    ili_cmd(0x36); ili_data8(0x48);
    ili_cmd(0x29); _delay_ms(20);
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return;
    ili_set_window(x, y, x, y);
    ili_data16(color);
}

void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return;
    if (x + w > ILI9341_WIDTH)  w = ILI9341_WIDTH  - x;
    if (y + h > ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

    ili_set_window(x, y, x + w - 1, y + h - 1);
    DC_DATA();
    CS_LOW();
    uint32_t total = (uint32_t)w * h;
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    for (uint32_t i = 0; i < total; i++) {
        spi_transfer(hi);
        spi_transfer(lo);
    }
    CS_HIGH();
}

void ili9341_fill_screen(uint16_t color) {
    ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}