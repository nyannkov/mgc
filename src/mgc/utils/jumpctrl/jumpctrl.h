/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_JUMPCTRL_H
#define MGC_JUMPCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

enum mgc_jump_state {
    MGC_JUMP_STATE_READY,
    MGC_JUMP_STATE_FLOATING,
    MGC_JUMP_STATE_JUMPING,
    MGC_JUMP_STATE_END
};

typedef struct mgc_jumpctrl {
    enum mgc_jump_state jump_state;
    uint16_t floating_duration;
    uint16_t floating_counter;
} mgc_jumpctrl_t;

void jumpctrl_init(mgc_jumpctrl_t *jumpctrl);
void jumpctrl_reset_jump_state(mgc_jumpctrl_t *jumpctrl);
void jumpctrl_set_floating_duration(mgc_jumpctrl_t *jumpctrl, uint16_t duration);
void jumpctrl_update_jump_state(mgc_jumpctrl_t *jumpctrl, bool jump_button_pressed, bool is_landing);
enum mgc_jump_state jumpctrl_get_jump_state(const mgc_jumpctrl_t *jumpctrl);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_JUMPCTRL_H*/
