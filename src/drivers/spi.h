#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// Initialisiert SPI als Master
void spi_init(void);

// Sendet ein Byte über SPI
void spi_send(uint8_t data);

// Sendet mehrere Bytes
void spi_send_buffer(uint8_t* data, uint16_t length);

#endif