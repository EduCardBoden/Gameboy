#include "spi.h"
#include <avr/io.h>

void spi_init(void) {
    
    DDRB |= (1 << PB5) | (1 << PB3) | (1 << PB2); // SCK (PB5), MOSI (PB3), CS (PB2) output

    SPCR = (1 << SPE) | (1 << MSTR); //enable SPI and Master in SPI-Control-Register
    SPSR = (1 << SPI2X); //double spped in SPI-Status-Register (8MHz)

}

uint8_t spi_transfer(uint8_t data) {
    SPDR = data; //every byte automatically clocked to MOSI-pin
    while (!(SPSR & (1 << SPIF))); //keep looping until nothing to write 
    return SPDR; //not needed for ILI9341 because nothing important gets written back
}