#include "pico/stdlib.h"
#include "pico/time.h"
#include "button.h"
#include "interrupt.h"

// inicializa um botão (sem interrupção)
void init_button(uint8_t buttonGPIO) {
    gpio_init(buttonGPIO);
    gpio_set_dir(buttonGPIO, GPIO_IN);
    // Adiciona um resistor no circuito para tornar a leitura mais precisa
    // Estados (padrão)
        // 1: se não estiver pressionado
        // 0: se pressionado
    gpio_pull_up(buttonGPIO);
}

// inicializa um botão (com interrupção)
void init_button_with_interrupt(uint buttonGPIO, uint32_t event, bool enabled) {
    init_button(buttonGPIO);
    gpio_set_irq_enabled_with_callback(buttonGPIO, event, true, &gpio_irq_handler);
}

// verifica se o botão foi pressionado
bool is_button_pressed(uint8_t buttonGPIO) {
    return !gpio_get(buttonGPIO);
}

// tratamento de debouncing do botão
bool debouncing(uint32_t ms){
    static uint32_t last_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_time < ms)
        return false;
    last_time = current_time;
    return true;

}

