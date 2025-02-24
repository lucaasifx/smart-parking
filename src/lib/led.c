#include "pico/stdlib.h"
#include "led.h"

// *********************** Lib padrao ***********************

// inicializa um led
void init_led(uint8_t LED) {
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    //inicia apagado
    turn_led_off(LED);
}

// liga um led
void turn_led_on(uint8_t LED) {
    gpio_put(LED, true);
}

// desliga um led
void turn_led_off(uint8_t LED) {
    gpio_put(LED, false);
}

// Realiza um blink em um LED
    // Recebe o intervalo e a quantidade de blinks que devem ser realizados
void blink_led(uint8_t LED, uint32_t ms, uint8_t cycles) {
    for(int i = 0; i < cycles; i++) {
        turn_led_on(LED);
        sleep_ms(ms);
        turn_led_off(LED);
        sleep_ms(ms);
    }
}