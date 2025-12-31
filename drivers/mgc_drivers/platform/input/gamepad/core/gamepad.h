/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_GAMEPAD_H
#define MGC_DRIVERS_GAMEPAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

enum mgc_gamepad_key {
    MGC_GAMEPAD_KEY_UP = 0,
    MGC_GAMEPAD_KEY_DOWN,
    MGC_GAMEPAD_KEY_RIGHT,
    MGC_GAMEPAD_KEY_LEFT,
    MGC_GAMEPAD_KEY_ENTER,
    MGC_GAMEPAD_KEY_CANCEL,
    MGC_GAMEPAD_KEY_MENU,
    MGC_GAMEPAD_KEY_HOME,
    MGC_GAMEPAD_KEY_CONTROL,
    MGC_GAMEPAD_KEY_UNKNOWN,
    MGC_GAMEPAD_KEY_COUNT
};

typedef struct mgc_gamepad_key_state {
    enum mgc_gamepad_key key;
    bool is_pressed;
    bool was_pressed;
    uint16_t hold_counter;
} mgc_gamepad_key_state_t;

typedef struct mgc_gamepad_source {
    bool (*get)(void *ctx, enum mgc_gamepad_key key);
    void (*poll)(void *ctx);
    void *ctx;
} mgc_gamepad_source_t;

typedef struct mgc_gamepad {
    mgc_gamepad_key_state_t *key_states;
    size_t key_count;
    mgc_gamepad_source_t *source;
} mgc_gamepad_t;

void gamepad_init(
    mgc_gamepad_t *gamepad,
    const mgc_gamepad_source_t *source,
    mgc_gamepad_key_state_t *key_states,
    size_t key_count
);
void gamepad_poll(mgc_gamepad_t *gamepad);
bool gamepad_is_pressed(const mgc_gamepad_t *gamepad, enum mgc_gamepad_key key);
bool gamepad_was_pressed(const mgc_gamepad_t *gamepad, enum mgc_gamepad_key key);
uint16_t gamepad_get_hold_counter(const mgc_gamepad_t *gamepad, enum mgc_gamepad_key key);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_GAMEPAD_H*/
