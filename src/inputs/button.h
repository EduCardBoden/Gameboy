#pragma once
#include <stdint.h>

typedef enum {
    BTN_LEFT  = 0,  // PD2
    BTN_RIGHT = 1,  // PD3
    BTN_ROT   = 2,  // PD4
    BTN_DOWN  = 3,  // PD5
} Button;

void button_init(void);
uint8_t button_is_pressed(Button btn);
uint8_t button_was_pressed(Button btn);