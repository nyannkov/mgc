/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_GAMEPAD_SAMPLE_H
#define MGC_GAMEPAD_SAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/gamepad/interface/gamepad_if.h"

const mgc_gamepad_if_t *gamepad_sample_get_instance(void);
void gamepad_sample_proc(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_GAMEPAD_SAMPLE_H*/
