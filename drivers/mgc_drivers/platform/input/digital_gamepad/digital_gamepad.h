/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_DIGITAL_GAMEPAD_H
#define MGC_DRIVERS_DIGITAL_GAMEPAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

enum mgc_digital_gamepad_key {
    MGC_DIGITAL_GAMEPAD_KEY_UP = 0,
    MGC_DIGITAL_GAMEPAD_KEY_DOWN,
    MGC_DIGITAL_GAMEPAD_KEY_RIGHT,
    MGC_DIGITAL_GAMEPAD_KEY_LEFT,
    MGC_DIGITAL_GAMEPAD_KEY_ENTER,
    MGC_DIGITAL_GAMEPAD_KEY_CANCEL,
    MGC_DIGITAL_GAMEPAD_KEY_MENU,
    MGC_DIGITAL_GAMEPAD_KEY_HOME,
    MGC_DIGITAL_GAMEPAD_KEY_CONTROL,
    MGC_DIGITAL_GAMEPAD_KEY_COUNT
};

typedef struct mgc_digital_gamepad_config {
    enum mgc_digital_gamepad_key key;
    int32_t pin;
    bool use_pull_up;
} mgc_digital_gamepad_config_t;

typedef struct mgc_digital_gamepad_state {
    const mgc_digital_gamepad_config_t *config;
    bool is_pressed;
    bool was_pressed;
    uint16_t hold_counter;
} mgc_digital_gamepad_state_t;

#define MGC_DIGITAL_GAMEPAD_INIT_STATE(config_ptr)   { .config = (config_ptr), .is_pressed = false, .was_pressed = false, .hold_counter = 0 }

typedef struct mgc_digital_gamepad {
    mgc_digital_gamepad_state_t* states;
    size_t num_states;
} mgc_digital_gamepad_t;

void digital_gamepad_init(mgc_digital_gamepad_t *gamepad, mgc_digital_gamepad_state_t *states, size_t num_states);
void digital_gamepad_proc(mgc_digital_gamepad_t *gamepad);
bool digital_gamepad_is_pressed(const mgc_digital_gamepad_t *gamepad, enum mgc_digital_gamepad_key key);
bool digital_gamepad_was_pressed(const mgc_digital_gamepad_t *gamepad, enum mgc_digital_gamepad_key key);
uint16_t digital_gamepad_get_hold_counter(const mgc_digital_gamepad_t *gamepad, enum mgc_digital_gamepad_key key);
size_t digital_gamepad_get_key_count(const mgc_digital_gamepad_t *gamepad);

#ifdef MGC_DIGITAL_GAMEPAD_USE_DEFAULT_PIN_CONFIG
void digital_gamepad_init_default_handler(void);
// Copies the pointer-based default handler to the caller-provided buffer.
// Must call digital_gamepad_init_default_handler() before this.
bool digital_gamepad_copy_default_handler(mgc_digital_gamepad_t* out);

#define MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT       MGC_DIGITAL_GAMEPAD_KEY_COUNT
extern const mgc_digital_gamepad_config_t mgc_digital_gamepad_default_config_table[MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT];
#endif/*MGC_DIGITAL_GAMEPAD_USE_DEFAULT_PIN_CONFIG*/

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_DIGITAL_GAMEPAD_H*/
