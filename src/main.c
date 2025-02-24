#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"


// ************************** LIBS ***********************
#include "lib/button.h"
#include "lib/led.h"
#include "lib/matrix.h"
#include "lib/ssd1306.h"
#include "interrupt.h"

// *******************************************************

int main() {
	stdio_init_all();
	init_matrix();

	
	init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);
	// o estacionamento inicia preenchido de vermelho
	set_matrix();

	
    while (true){
		sleep_ms(1000);
    }

    return 0;
}