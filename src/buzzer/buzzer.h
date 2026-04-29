#pragma once 

#include <avr/io.h>
#include <stdint.h>

#define BUZZER_DDR   DDRB
#define BUZZER_PORT  PORTB
#define BUZZER_PIN   PB1

//musik Notenfrequenzen definieren
#define NOTE_E5  659
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_A4  440
#define NOTE_A5  880
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_E4  330
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_REST  0

//zeitkonstanten definieren
#define DUR_QUARTER      400
#define DUR_HALF         800
#define DUR_EIGHTH       200
#define DUR_DOT_QUARTER  600

typedef struct {

    uint16_t freq; //frequenz der Note
    uint16_t dur_ms; //duration der note
} BuzzerNote;

void buzzer_init(void);
void buzzer_play(const BuzzerNote *song, uint8_t length);
void buzzer_stop(void);


