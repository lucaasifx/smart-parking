#ifndef LED_H
#define LED_H


#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13

// funções padrão
void init_led(uint8_t ledGPIO);
void turn_led_on(uint8_t ledGPIO);
void turn_led_off(uint8_t ledGPIO);
void blink_led(uint8_t ledGPIO, uint32_t ms, uint8_t cycles);


#endif