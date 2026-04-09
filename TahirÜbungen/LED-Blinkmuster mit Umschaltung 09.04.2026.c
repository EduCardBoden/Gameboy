#include <avr/io.h> 
#include <util/delay.h> 
 

int main(void) 
{ 
     
    DDRC = 0xFF;    
    PORTC = 0x01;

while(1)
{
    while (!(PORTC&(1<<PORTC3)))

    {
        PORTC = (PORTC << 1) | (PORTC >> 3); 
        _delay_ms(500); 
    }

}
