#include <avr/io.h> 
#include <util/delay.h> 
 

int main(void) 
{ 
     
    DDRC = 0x0F;    
    while (1) 
    { 
        for (int i=0; i<5; i++) 
        { 
            PORTC = (0x0F); 
            _delay_ms(200); 
            PORTC = (0x00); 
            _delay_ms(200);
        }
        for (int i=0; i<5; i++) 
        { 
            PORTC = (0x05); 
            _delay_ms(200); 
            PORTC = (0x0A); 
            _delay_ms(200);
        }
    }
}