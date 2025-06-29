/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "digital_gamepad.h"
#include "ports/digital_gamepad_port.h"

#define MAX_KEY_HOLD_COUNTER        UINT16_MAX

#ifdef MGC_DIGITAL_GAMEPAD_USE_DEFAULT_PIN_CONFIG

/*****************************************************************************
* Default configuration for Waveshare Pico-LCD-1.3
*
* Reference:
* https://www.waveshare.com/wiki/Pico-LCD-1.3
******************************************************************************/
const mgc_digital_gamepad_config_t mgc_digital_gamepad_default_config_table[MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT] = {
    { MGC_DIGITAL_GAMEPAD_KEY_UP,       2, true },
    { MGC_DIGITAL_GAMEPAD_KEY_DOWN,    18, true },
    { MGC_DIGITAL_GAMEPAD_KEY_RIGHT,   20, true },
    { MGC_DIGITAL_GAMEPAD_KEY_LEFT,    16, true },
    { MGC_DIGITAL_GAMEPAD_KEY_ENTER,   15, true }, // keyA
    { MGC_DIGITAL_GAMEPAD_KEY_CANCEL,  17, true }, // keyB
    { MGC_DIGITAL_GAMEPAD_KEY_MENU,    19, true }, // keyX
    { MGC_DIGITAL_GAMEPAD_KEY_HOME,    21, true }, // keyY
    { MGC_DIGITAL_GAMEPAD_KEY_CONTROL,  3, true }
};

static mgc_digital_gamepad_state_t default_state_table[MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT] = {
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[0]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[1]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[2]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[3]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[4]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[5]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[6]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[7]),
    MGC_DIGITAL_GAMEPAD_INIT_STATE(&mgc_digital_gamepad_default_config_table[8])
};

static mgc_digital_gamepad_t default_handler = {
    .states = default_state_table,
    .num_states = MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT
};

void digital_gamepad_init_default_handler(void) {
    digital_gamepad_init(&default_handler, default_state_table, countof(default_state_table));
}

bool digital_gamepad_copy_default_handler(mgc_digital_gamepad_t* out) {
    if ( out == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    out->states = default_handler.states;
    out->num_states = default_handler.num_states;

    return true;
}

#endif/*MGC_DIGITAL_GAMEPAD_USE_DEFAULT_PIN_CONFIG*/

static const mgc_digital_gamepad_state_t* get_state_by_key(const mgc_digital_gamepad_t *gamepad, enum mgc_digital_gamepad_key key) {
    for ( size_t i = 0; i < gamepad->num_states; i++ ) {
        mgc_digital_gamepad_state_t *state = &gamepad->states[i];
        if ( state->config != NULL ) {
            if ( state->config->key == key ) {
                return state;
            }
        }
    }
    return NULL;
}

void digital_gamepad_init(mgc_digital_gamepad_t *gamepad, mgc_digital_gamepad_state_t *states, size_t num_states) {
    if ( ( gamepad == NULL ) ||
         ( states == NULL ) ||
         ( num_states == 0 )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    gamepad->states = states;
    gamepad->num_states = num_states;

    for ( size_t i = 0; i < num_states; i++ ) {
        mgc_digital_gamepad_state_t *state = &gamepad->states[i];
        if ( state->config != NULL ) {
            digital_gamepad_port__gpio_init(state->config->pin);
            digital_gamepad_port__gpio_set_input_mode(state->config->pin, state->config->use_pull_up);
        }
        state->is_pressed = false;
        state->was_pressed = false;
        state->hold_counter = 0;
    }
}

void digital_gamepad_proc(mgc_digital_gamepad_t *gamepad) {
    if ( ( gamepad == NULL ) ||
         ( gamepad->states == NULL ) ||
         ( gamepad->num_states == 0 )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    for ( size_t i = 0; i < gamepad->num_states; i++ ) {
        mgc_digital_gamepad_state_t *state = &gamepad->states[i];
        state->was_pressed = state->is_pressed;
        if ( state->config != NULL ) {
            state->is_pressed = ( digital_gamepad_port__gpio_get(state->config->pin) == false );
        }
        if ( state->is_pressed == true ) {
            if ( state->hold_counter < MAX_KEY_HOLD_COUNTER ) {
                state->hold_counter++;
            }
        } else {
            state->hold_counter = 0;
        }
    }
}

bool digital_gamepad_is_pressed(const mgc_digital_gamepad_t *gamepad, enum mgc_digital_gamepad_key key) {
    if ( gamepad == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    const mgc_digital_gamepad_state_t* state = get_state_by_key(gamepad, key);
    return ( state != NULL ) ? state->is_pressed : false;
}

bool digital_gamepad_was_pressed(const mgc_digital_gamepad_t *gamepad, enum mgc_digital_gamepad_key key) {
    if ( gamepad == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    const mgc_digital_gamepad_state_t* state = get_state_by_key(gamepad, key);
    return ( state != NULL ) ? state->was_pressed : false;
}

uint16_t digital_gamepad_get_hold_counter(const mgc_digital_gamepad_t *gamepad, enum mgc_digital_gamepad_key key) {
    if ( gamepad == NULL ) {
        MGC_WARN("Invalid handler");
        return 0;
    }

    const mgc_digital_gamepad_state_t* state = get_state_by_key(gamepad, key);
    return ( state != NULL ) ? state->hold_counter : 0;
}

size_t digital_gamepad_get_key_count(const mgc_digital_gamepad_t *gamepad) {
    if ( gamepad == NULL ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    return gamepad->num_states;
}

