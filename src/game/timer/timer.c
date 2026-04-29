#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "../game.h"
#include "../../buzzer/buzzer.h"  //neu

volatile uint8_t tick_count = 0;  //zählt wie oft der ISR aufgerufen wurde

void timer_init(void) {

    TCCR2A |= (1 << WGM21);                             //CTC mode (zähl bis OCR2A dann wieder 0)
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);  //prescaler 1024 -> ein schritt = 16M / 1024 = 15.625 schritte pro sek -> 1 schritt = 0.064ms
    OCR2A = 77;                                         //alle ~5ms ein ISR-Aufruf
}

ISR(TIMER2_COMPA_vect) { //Interrupt Service Routine (TIMER2_COMPA_vect ist name vom timer2 interrupts)

    

    tick_count++;
    if (tick_count >= 100) {  //100 x 5ms = 500ms
        tick_count = 0;

        if (game_state == GAME_PLAY) {  //only tick during gameplay
            game_tick();
        }
    }
}