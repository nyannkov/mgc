/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "jumpctrl.h"

void jumpctrl_init(mgc_jumpctrl_t *jumpctrl) {
    if ( jumpctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    jumpctrl->jump_state = MGC_JUMP_STATE_READY;
    jumpctrl->floating_duration = 3;
    jumpctrl->floating_counter = 0;
}

void jumpctrl_reset_jump_state(mgc_jumpctrl_t *jumpctrl) {
    if ( jumpctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    jumpctrl->jump_state = MGC_JUMP_STATE_READY;
    jumpctrl->floating_counter = 0;
}

void jumpctrl_set_floating_duration(mgc_jumpctrl_t *jumpctrl, uint16_t duration) {
    if ( jumpctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    jumpctrl->floating_duration = duration;
    jumpctrl->floating_counter = 0;
}

void jumpctrl_update_jump_state(mgc_jumpctrl_t *jumpctrl, bool jump_button_pressed, bool is_landing) {
    if ( jumpctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    enum mgc_jump_state jump_state = jumpctrl->jump_state;
    enum mgc_jump_state next_state = jump_state;

    switch (jump_state) {
    case MGC_JUMP_STATE_READY:
        if ( is_landing && jump_button_pressed ) {
            next_state = MGC_JUMP_STATE_FLOATING;
            jumpctrl->floating_counter = jumpctrl->floating_duration;
        }
        break;

    case MGC_JUMP_STATE_FLOATING:
        if ( ( jumpctrl->floating_counter-- <= 0 ) ||
             ( !jump_button_pressed )
        ) {
            next_state = MGC_JUMP_STATE_JUMPING;
        }
        break;

    case MGC_JUMP_STATE_JUMPING:
        if ( is_landing ) {
            next_state = MGC_JUMP_STATE_END;
        }
        break;

    case MGC_JUMP_STATE_END:
        if ( !jump_button_pressed ) {
            next_state = MGC_JUMP_STATE_READY;
        }
        break;

    default:
        break;
    }

    if ( jump_state != next_state ) {
        jumpctrl->jump_state = next_state;
    }
}

enum mgc_jump_state jumpctrl_get_jump_state(const mgc_jumpctrl_t *jumpctrl) {
    if ( jumpctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return MGC_JUMP_STATE_END;
    }
    return jumpctrl->jump_state;
}
