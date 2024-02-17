/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_GAMEPAD_RP2040_H
#define MGC_GAMEPAD_RP2040_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/gamepad/interface/gamepad_if.h"

extern const struct mgc_gamepad_if    game_pad_rp2040;

void game_pad_rp2040_proc(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_GAMEPAD_RP2040_H*/
