#pragma once


#include <stdint.h> 

void    spi_init(void); //set SPI hardware on microcontroller
uint8_t spi_transfer(uint8_t data); //send data to screen

