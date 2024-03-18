/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SYSTEM_H
#define MGC_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/display/interface/display_if.h"
#include "mgc/gamepad/interface/gamepad_if.h"
#include "mgc/font/font.h"

const uint16_t sys_get_display_width(void);
const uint16_t sys_get_display_height(void);
const mgc_display_if_t *sys_get_display_driver(void);
const mgc_gamepad_if_t *sys_get_gamepad_driver(void);
void sys_gamepad_proc(void);
const mgc_font_t *sys_get_font(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SYSTEM_H*/
