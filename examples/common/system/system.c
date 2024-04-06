/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "system.h"
#include "mgc/display/lcd/display_lcd.h"
#include "mgc/gamepad/sample/gamepad_sample.h"
#include "resources/generates/font/k8x12S.h"

const mgc_display_if_t *sys_get_display_driver(void) {
    return display_lcd_get_instance();
}

const mgc_gamepad_if_t *sys_get_gamepad_driver(void) {
    return gamepad_sample_get_instance();
}

void sys_gamepad_proc(void) {
    gamepad_sample_proc();
}

const mgc_font_t *sys_get_font(void) {
    return &k8x12S;
}

