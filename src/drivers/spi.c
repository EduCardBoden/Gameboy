#include <avr/io.h>
#include "spi.h"

void spi_init(void)
{
    // MOSI (PB3), SCK (PB5), SS (PB2) als Output
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);

    // MISO (PB4) als Input
    DDRB &= ~(1 << PB4);

    // SS auf HIGH setzen (wichtig für Master Mode)
    PORTB |= (1 << PB2);

    // SPI aktivieren, Master, Takt = f/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_send(uint8_t data)
{
    SPDR = data; // Daten in Register schreiben

    // Warten bis Übertragung fertig
    while (!(SPSR & (1 << SPIF)));
}

void spi_send_buffer(uint8_t* data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        spi_send(data[i]);
    }
}