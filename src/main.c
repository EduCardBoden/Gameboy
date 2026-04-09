#include "avr/io.h"

int main(void)
{
    spi_init();

    while (1)
    {
        spi_send(0xAA);
    }
}