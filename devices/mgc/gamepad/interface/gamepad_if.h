/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_GAME_PAD_H
#define MGC_GAME_PAD_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "mgc/common/common.h"

enum gp_key {
    GP_KEY_UP,
    GP_KEY_DOWN,
    GP_KEY_LEFT,
    GP_KEY_RIGHT,
    GP_KEY_0,
    GP_KEY_1,
    GP_KEY_2,
    GP_KEY_3,
    GP_KEY_4,
    GP_KEY_5,
    GP_KEY_6,
    GP_KEY_7,
    GP_KEY_8,
    GP_KEY_9,
};

enum gp_key_state {
    GP_KEY_STATE_OFF,
    GP_KEY_STATE_ON,
};

enum gp_firing_state {
    GP_FIRING_OFF,
    GP_FIRING_ON,
};

typedef struct mgc_gamepad_if {
    int (*init)(void);
    int16_t (*get_x_axis)(void);
    int16_t (*get_y_axis)(void);
    enum gp_key_state (*get_key_state)(enum gp_key key);
    bool (*is_key_state_changed)(enum gp_key key);
    bool (*is_key_on_edge)(enum gp_key key);
    bool (*is_key_off_edge)(enum gp_key key);
    enum gp_firing_state (*get_firing_state)(enum gp_key key);
} mgc_gamepad_if_t;

#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif/*MGC_GAME_PAD_H*/
