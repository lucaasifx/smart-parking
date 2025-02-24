#include "interrupt.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"
#include "ssd1306.h"
#include "led.h"
#include "button.h"
#include "matrix.h"
#include <stdio.h>

// inicia ativado
volatile bool PWM = true;

// Função que lida com eventos de interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
    // eventos de acionamento de botões
    if(gpio == BUTTON_A || gpio == BUTTON_B || gpio == JOYSTICK_BUTTON) {
        // tratamento de debouncing dos botões
        if(!debouncing(300))
            return;
        // alterna o estado do led verde
        if(gpio == JOYSTICK_BUTTON) {
            clear_matrix();
            // Entra no modo BOOTSELL e encerra o programa
            printf("\nFinalizando o programa...\n");
            reset_usb_boot(0,0);
        }

        if(gpio == BUTTON_A) {
        }

        if(gpio == BUTTON_B) {
        }

    }
}