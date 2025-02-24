#ifndef INTERRUPT_H
#define INTERRUPT_H


#include "pico/stdlib.h"

extern volatile bool PWM;

void gpio_irq_handler(uint gpio, uint32_t events);

#endif
