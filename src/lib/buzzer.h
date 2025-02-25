#ifndef BUZZER_H
#define BUZZER_H


#define BUZZER_01 21
#define BUZZER_02 10

#include "pico/stdlib.h"

extern bool buzzer_state;

void pwm_init_buzzer(uint pin);
void play_tone(uint pin, uint frequency, uint duration_ms);

#endif