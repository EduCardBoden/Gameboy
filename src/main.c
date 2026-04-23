#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// --- PIN MAKROS ---
#define CS_LOW()   PORTB &= ~(1 << PB2)
#define CS_HIGH()  PORTB |= (1 << PB2)

#define DC_LOW()   PORTB &= ~(1 << PB0)
#define DC_HIGH()  PORTB |= (1 << PB0)

#define RST_LOW()  PORTB &= ~(1 << PB1)
#define RST_HIGH() PORTB |= (1 << PB1)

// --- SPI ---
void SPI_Send(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

// --- COMMAND ---
void TFT_Command(uint8_t cmd) {
    DC_LOW();
    CS_LOW();
    SPI_Send(cmd);
    CS_HIGH();
}

// --- DATA ---
void TFT_Data(uint8_t data) {
    DC_HIGH();
    CS_LOW();
    SPI_Send(data);
    CS_HIGH();
}

// --- INIT SPI ---
void SPI_Init() {
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2) | (1 << PB0) | (1 << PB1);
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);
}

// --- RESET ---
void TFT_Reset() {
    RST_LOW();
    _delay_ms(50);
    RST_HIGH();
    _delay_ms(150);
}

// --- SCREEN GRÜN ---
void TFT_FillGreen() {
    TFT_Command(0x2A);
    TFT_Data(0x00); TFT_Data(0x00);
    TFT_Data(0x00); TFT_Data(0xEF);

    TFT_Command(0x2B);
    TFT_Data(0x00); TFT_Data(0x00);
    TFT_Data(0x01); TFT_Data(0x3F);

    TFT_Command(0x2C);

    DC_HIGH();
    CS_LOW();

    for (uint32_t i = 0; i < 240UL * 320UL; i++) {
        SPI_Send(0x07); // 🟢 Grün high byte
        SPI_Send(0xE0); // 🟢 Grün low byte
    }

    CS_HIGH();
}

// --- MAIN ---
int main(void) {
    SPI_Init();
    TFT_Reset();

    TFT_Command(0x11);
    _delay_ms(120);

    TFT_Command(0x3A);
    TFT_Data(0x55);

    TFT_Command(0x29);

    TFT_FillGreen(); // 🟢 Bildschirm grün

    while (1) {}
}