#include "matrix.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"


bool led_buffer[MATRIX_LEN] = {
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0
};

void init_matrix() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, MATRIX_PIN, 800000, IS_RGBW);
}


void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_matrix() {
    uint32_t avaible = urgb_u32(0, GREEN_INTENSITY, 0);
    uint32_t unavaible = urgb_u32(RED_INTENSITY, 0, 0);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < MATRIX_LEN; i++) {
        if (led_buffer[i])
            put_pixel(avaible); // se a vaga estiver livre o led está verde 
        else
            put_pixel(unavaible); // se a vaga estiver ocupada o led está vermelho
    }
}

void set_led(int pos, bool state) {
	if(pos == -1)
		return;
	led_buffer[pos] = state;
}

int get_index(int position) {
    if (position < 1 || position > 25)
        return -1;
    position--; // Ajusta para base 0
    uint8_t row = position / 5;
    uint8_t col = position % 5;
  
    // Se a linha for par (0, 2, 4), LEDs estão da direita para a esquerda
    if(row % 2 == 0)
		return (row * 5) + (4 - col);
    // Se a linha for ímpar (1, 3), LEDs estão da esquerda para a direita
    else
		return (row * 5) + col;
}

void reset_matrix() {
    // Zera o buffer lógico (marca todos os LEDs como ocupados)
    for (int i = 0; i < MATRIX_LEN; i++)
        led_buffer[i] = false;

    // Envia os dados ao LED (todos vermelhos)
    set_matrix();
}

void clear_matrix() {
    // Zera o buffer lógico (marca todos os LEDs como ocupados)
    for (int i = 0; i < MATRIX_LEN; i++) {
        led_buffer[i] = false;
        put_pixel(0);
    }
}

bool get_state(int index) {
    return led_buffer[index];
}

