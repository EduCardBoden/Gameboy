#define F_CPU 16000000UL
#include "button.h"
#include <avr/io.h>
#include <util/delay.h>

static const uint8_t btn_pins[4] = {PD2, PD3, PD4, PD5}; //alle buttons auf port d

void button_init(void) {
    
    DDRD  &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5));  //alle input
    PORTD |=  ((1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5));  //alle pull-up
}

uint8_t button_is_pressed(Button btn) {

    if (!(PIND & (1 << btn_pins[btn]))) {

        return 1; //gedrückt
    }
    return 0; //nicht gedrückt
}

uint8_t button_was_pressed(Button btn) {

    if (!(PIND & (1 << btn_pins[btn]))) {

        _delay_ms(20); //debouncing
        if (!(PIND & (1 << btn_pins[btn]))) {

            while (!(PIND & (1 << btn_pins[btn]))) {} //warten bis release
            return 1;
        }
    }
    return 0;
}