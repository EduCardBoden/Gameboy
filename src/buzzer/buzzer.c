#define F_CPU 16000000UL
#include "buzzer.h"
#include <avr/io.h>
#include <util/delay.h>

#define TIMER_CLOCK (F_CPU / 8UL) //2MHz -> bei dieser geschwindigkeit runned der timer

//static functions
static void tone_start(uint16_t freq) {

    uint16_t top = (TIMER_CLOCK / freq) - 1; //counter bis zu der Timer1 zählt bis zum toggeln vom pin
    OCR1A  = top; //loaded den target-count ins output compare register 
    TCNT1  = 0; //reset zu 0 für nächsten note
    TCCR1A = (1 << COM1A0); //automatisch zu PB1 toggeln bei compare match
    TCCR1B = (1 << WGM12) | (1 << CS11); //set CTC mode (Clear Timer on Compare) und set prescaler zu 8
}

static void tone_stop(void) {

    TCCR1A = 0; //timer and counter control register A
    TCCR1B = 0; //timer and counter control register B
    BUZZER_PORT &= ~(1 << BUZZER_PIN);
}

//public functions
void buzzer_init(void) {

    BUZZER_DDR |= (1 << BUZZER_PIN); //set PB1 output
    tone_stop(); //silent start-up
}

void buzzer_play(const BuzzerNote *song, uint8_t length) {

    for(uint8_t i = 0; i < length; i++){ //alle notes im array durchgehen

           uint16_t gap = song[i].dur_ms / 10; //10% silence gap zwischen notes um legato blur zu preventen
  
        if (song[i].freq == NOTE_REST) { //wenn freq = 0, pause/rest

            tone_stop(); //safety check ob buzzer still ist
            for (uint16_t ms = 0; ms < song[i].dur_ms; ms++){

                _delay_ms(1); //ganze rest dauer warten
            }
        } 

        else { //normale note

            tone_start(song[i].freq); //start timer, buzzer spielt

            for (uint16_t ms = 0; ms < song[i].dur_ms - gap; ms++){

                _delay_ms(1); //warten für dur - gap (hörbare teil)
            }
            tone_stop(); //note stoppen
                
            for (uint16_t ms = 0; ms < gap; ms++) {

                _delay_ms(1);  //warten für gap = stille zwischen notes
            }
        }
    }
}
 