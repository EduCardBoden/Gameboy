#include "spi.h"
#include <avr/io.h>

void spi_init(void) {
    
    DDRD |= (1 << PB5) | (1 << PB3) | (1 << PB2); // SCK (PB5), MOSI (PB3), CS (PB2) output

    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);

}

uint8_t spi_transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}