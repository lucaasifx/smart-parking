#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "ws2812.pio.h"


// ************************** LIBS ***********************
#include "lib/button.h"
#include "lib/buzzer.h"
#include "lib/led.h"
#include "lib/matrix.h"
#include "lib/ssd1306.h"
#include "interrupt.h"

// *******************************************************

#define BUZZER_FREQUENCY 784
#define INITIAL_AVAIBLE_PARKING_SPACES 8
#define VRY_PIN 26
#define DEAD_ZONE 500
#define CENTER_JS 2048

// Configurações da I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

typedef struct {
	int num_options[7];  // Número de opções por página
    char (*pages[7])[10]; // Ponteiro para um array de strings de 10 caracteres
    int current_page;     // Página atual
} Pages;

char page1[4][10] = {"VAGA 1", "VAGA 2", "VAGA 3", "VAGA 4"};
char page2[4][10] = {"VAGA 5", "VAGA 6", "VAGA 7", "VAGA 8"};
char page3[4][10] = {"VAGA 9", "VAGA 10", "VAGA 11", "VAGA 12"};
char page4[4][10] = {"VAGA 13", "VAGA 14", "VAGA 15", "VAGA 16"};
char page5[4][10] = {"VAGA 17", "VAGA 18", "VAGA 19", "VAGA 20"};
char page6[4][10] = {"VAGA 21", "VAGA 22", "VAGA 23", "VAGA 24"};
char page7[1][10] = {"VAGA 25"};

uint16_t joystick_read_value(uint8_t jsGPIO) {
    adc_select_input(jsGPIO - 26);
    uint16_t axis_value = adc_read();
    return axis_value;
}


int get_random_index() {
    return rand() % 25;
}

int main() {
	stdio_init_all();
	sleep_ms(100);
	// semente para geração de números aleatórios
	srand(time_us_32());
	init_matrix();

	adc_init();
    adc_gpio_init(VRY_PIN);

    // Inicializa I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);



	// reset do programa com o botão do joystick
	init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);
	init_button_with_interrupt(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);
	pwm_init_buzzer(BUZZER_01);
	// o estacionamento inicia preenchido de vermelho
	set_matrix();
	sleep_ms(50);

	int init_random_parkings = 0;
	while(init_random_parkings < INITIAL_AVAIBLE_PARKING_SPACES) {
		int randnum = get_random_index() + 1;
		if(!get_state(get_index(randnum))) {
			set_led(get_index(randnum), true);
			init_random_parkings++;
		}
	}
	// escreve o estado atual das vagas na matriz
	set_matrix();
	sleep_ms(50);

    Pages menu = {
        .num_options = {4, 4, 4, 4, 4, 4, 1},
        .pages = {page1, page2, page3, page4, page5, page6, page7},
        .current_page = 0 // Começa na primeira página
    };


    while (true) {
		// limpa a tela
		ssd1306_fill(&ssd, false);
		// lê os valores do eixo y do joystick
        uint16_t vry_value = joystick_read_value(VRY_PIN);

		switch (screen_state){
			case Parking_Selection: {
				// move pra baixo
				if(vry_value < CENTER_JS - DEAD_ZONE) {
					parking_option++;
					// Se ultrapassar as opções da página atual
					if(parking_option >= menu.num_options[menu.current_page]) {
						// Se estiver na última página, volta para a primeira página e opção 0
						if(menu.current_page == 6) { 
							menu.current_page = 0;
							parking_option = 0;
						} 
						else {
							// Caso contrário, avança para a próxima página e reseta a opção
							parking_option = 0;
							menu.current_page++;
						}
					}
					// Atualiza a vaga selecionada corretamente aqui!
					selected_parking = (menu.current_page * 4) + parking_option + 1;
					play_tone(BUZZER_01, BUZZER_FREQUENCY, 100);
					sleep_ms(200);
				}
			
				// move pra cima
				if(vry_value > CENTER_JS + DEAD_ZONE) {
					// Se estiver na primeira opção da primeira página, volta para a última opção da última página
					if(!menu.current_page && !parking_option) {
						menu.current_page = 6;  // Última página
						parking_option = menu.num_options[menu.current_page] - 1;
					} 
					else {
						// Se estiver na primeira opção da página, volta para a página anterior
						if(!parking_option) {
							menu.current_page--;
							parking_option = menu.num_options[menu.current_page] - 1;
						} 
						else
							parking_option--;
					}
					// Atualiza a vaga selecionada corretamente aqui!
				selected_parking = (menu.current_page * 4) + parking_option + 1;
					play_tone(BUZZER_01, BUZZER_FREQUENCY, 100);
					sleep_ms(200);
				}
			
				draw_border(&ssd);
				int offset = 4;
				for(int i = 0; i < menu.num_options[menu.current_page]; i++) {
					ssd1306_draw_string(&ssd, menu.pages[menu.current_page][i], 40, offset);
					offset += 16;
				}
				ssd1306_send_data(&ssd);
				break;
			}
			
			case Parking_Unavaible: {
				char buffer[20];
				sprintf(buffer, "VAGA %d", selected_parking);
				int text_width = strlen(buffer) * 8;
				int x_center = (128 - text_width) / 2;
				ssd1306_draw_string(&ssd, buffer, x_center, 20);
				int text_width2 = strlen("INDISPONIVEL") * 8;
				int x_center2 = (128 - text_width2) / 2;
				ssd1306_draw_string(&ssd, "INDISPONIVEL", x_center2, 40);
				ssd1306_send_data(&ssd);
				play_tone(BUZZER_01, BUZZER_FREQUENCY, 1000);

				screen_state = Parking_Selection;
				break;
			}
			
			default:
				break;
		}
		sleep_ms(50);
		
    }

    return 0;
}