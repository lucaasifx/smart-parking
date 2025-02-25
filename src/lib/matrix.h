#ifndef MATRIX_H
#define MATRIX_H

#include "pico/stdlib.h"
#include "hardware/pio.h"

#define IS_RGBW false
#define MATRIX_LEN 25
#define MATRIX_PIN 7

#define RED_INTENSITY 1
#define GREEN_INTENSITY 1

extern bool led_buffer[MATRIX_LEN];

void init_matrix();
void put_pixel(uint32_t pixel_grb);
void set_matrix();
void reset_matrix();
void clear_matrix();
void set_led(int pos, bool state);
int get_index(int pos);
bool get_state(int index);

#endif