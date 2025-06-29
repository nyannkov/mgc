/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_DIGITAL_GAMEPAD_PORT_H
#define MGC_DRIVERS_DIGITAL_GAMEPAD_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

void digital_gamepad_port__gpio_init(unsigned int pin);
void digital_gamepad_port__gpio_set_input_mode(unsigned int pin, bool pullup);
bool digital_gamepad_port__gpio_get(unsigned int pin);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_DIGITAL_GAMEPAD_PORT_H*/
