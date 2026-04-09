#include "avr/io.h"

int main(void)
{
    DDRB |= (1 << DDB5); // Set pin 13 as output

    while (1)
    {
        PORTB ^= (1 << PORTB5); // Toggle pin 13
        for (volatile uint32_t i = 0; i < 100000; i++); // Delay
    }

    return 0;
}