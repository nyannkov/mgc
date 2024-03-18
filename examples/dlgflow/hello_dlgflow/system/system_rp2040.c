/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "system.h"
#include "mgc/display/ili9341/rp2040/display_ili9341_rp2040.h"
#include "mgc/gamepad/rp2040/gamepad_rp2040.h"
#include "resources/generates/font/k8x12S.h"

#define DISPLAY_WIDTH_PIXEL         (240)
#define DISPLAY_HEIGHT_PIXEL        (320)

const uint16_t sys_get_display_width(void) {
    return DISPLAY_WIDTH_PIXEL;
}

const uint16_t sys_get_display_height(void) {
    return DISPLAY_HEIGHT_PIXEL;
}

const mgc_display_if_t *sys_get_display_driver(void) {
    return &display_driver_ili9341_rp2040;
}

const mgc_gamepad_if_t *sys_get_gamepad_driver(void) {
    return &game_pad_rp2040;
}

void sys_gamepad_proc(void) {
    game_pad_rp2040_proc();
}

const mgc_font_t *sys_get_font(void) {
    return &k8x12S;
}

