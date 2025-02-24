#ifndef BUTTON_H
#define BUTTON_H


#define BUTTON_A 5
#define BUTTON_B 6
#define JOYSTICK_BUTTON 22

#include "pico/stdlib.h"

void init_button(uint8_t buttonGPIO);
bool is_button_pressed(uint8_t buttonGPIO);
void init_button_with_interrupt(uint buttonGPIO, uint32_t event, bool enabled);
bool debouncing();

#endif