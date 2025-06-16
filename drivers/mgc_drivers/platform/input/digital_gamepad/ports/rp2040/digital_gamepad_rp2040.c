/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "hardware/gpio.h"

void digital_gamepad_port__gpio_init(int pin) {
    gpio_init(pin);
}

void digital_gamepad_port__gpio_set_input_mode(unsigned int pin, bool pull_up) {
    gpio_set_dir(pin, GPIO_IN);
    if ( pull_up == true ) {
        gpio_pull_up(pin);
    }
}

bool digital_gamepad_port__gpio_get(unsigned int pin) {
    return gpio_get(pin);
}

