/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "gamepad.h"

#define MAX_KEY_HOLD_COUNTER        UINT16_MAX

static const mgc_gamepad_key_state_t* get_state_by_key(const mgc_gamepad_t *gamepad, enum mgc_gamepad_key key) {
    for ( size_t i = 0; i < gamepad->key_count; i++ ) {
        mgc_gamepad_key_state_t *key_state = &gamepad->key_states[i];
        if ( key_state->key == key ) {
            return key_state;
        }
    }
    return NULL;
}

void gamepad_init(
    mgc_gamepad_t *gamepad,
    mgc_gamepad_source_t *source,
    mgc_gamepad_key_state_t *key_states,
    size_t key_count
) {
    if ( ( gamepad == NULL ) ||
         ( source == NULL ) ||
         ( source->get == NULL ) ||
         ( key_states == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    gamepad->key_states = key_states;
    gamepad->key_count = key_count;
    gamepad->source = source;

    for ( size_t i = 0; i < key_count; i++ ) {
        key_states[i].is_pressed = false;
        key_states[i].was_pressed = false;
    }
}

void gamepad_poll(mgc_gamepad_t *gamepad) {
    if ( ( gamepad == NULL ) ||
         ( gamepad->source == NULL ) ||
         ( gamepad->source->get == NULL ) ||
         ( gamepad->key_states == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    if ( gamepad->source->poll ) {
        gamepad->source->poll(gamepad->source->ctx);
    }

    mgc_gamepad_source_t *source = gamepad->source;

    for ( size_t i = 0; i < gamepad->key_count; i++ ) {
        mgc_gamepad_key_state_t *key_states;
        key_states = &gamepad->key_states[i];

        key_states->was_pressed = key_states->is_pressed;
        key_states->is_pressed = source->get(source->ctx, key_states->key);

        if ( key_states->is_pressed == true ) {
            if ( key_states->hold_counter < MAX_KEY_HOLD_COUNTER ) {
                key_states->hold_counter++;
            }
        } else {
            key_states->hold_counter = 0;
        }
    }
}

bool gamepad_is_pressed(const mgc_gamepad_t *gamepad, enum mgc_gamepad_key key) {
    if ( ( gamepad == NULL ) ||
         ( gamepad->key_states == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    
    const mgc_gamepad_key_state_t *key_states = get_state_by_key(gamepad, key);

    if ( key_states == NULL ) {
        MGC_WARN("Gamepad key is not found");
        return false;
    }

    return key_states->is_pressed;
}

bool gamepad_was_pressed(const mgc_gamepad_t *gamepad, enum mgc_gamepad_key key) {
    if ( ( gamepad == NULL ) ||
         ( gamepad->key_states == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    
    const mgc_gamepad_key_state_t *key_states = get_state_by_key(gamepad, key);

    if ( key_states == NULL ) {
        MGC_WARN("Gamepad key is not found");
        return false;
    }

    return key_states->was_pressed;
}

uint16_t gamepad_get_hold_counter(const mgc_gamepad_t *gamepad, enum mgc_gamepad_key key) {
    if ( ( gamepad == NULL ) ||
         ( gamepad->key_states == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return 0;
    }

    const mgc_gamepad_key_state_t *key_states = get_state_by_key(gamepad, key);

    if ( key_states == NULL ) {
        MGC_WARN("Gamepad key is not found");
        return 0;
    }

    return key_states->hold_counter;
}

