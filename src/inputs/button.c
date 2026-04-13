#include "button.h"
#include <avr/io.h>
#include <util/delay.h>


void button_init(void) {

    DDRD &= ~(1 << PD2); //PD2 input
    PORTD |= (1 << PD2); //pull-up

}

uint8_t button_is_pressed(void) {

    if(!(PIND & (1 << PD2))) //gedrückt oder nicht?
    {
     return 1; 
    }
    return 0;
}

uint8_t button_was_pressed(void) {

    if(!(PIND & (1 << PD2))) {

        _delay_ms(20); //check nach 20ms ob noch immer pressed (debouncing)

        if(!(PIND & (1 << PD2))) {

            while(!(PIND & (1 << PD2))) { //warte bis button-release
            }
             return 1;
        }
    }
    return 0;
}
    


