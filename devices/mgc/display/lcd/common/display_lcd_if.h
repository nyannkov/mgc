/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DISPLAY_LCD_IF_H
#define MGC_DISPLAY_LCD_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/display/interface/display_if.h"

typedef struct mgc_display_lcd_if {
    const mgc_display_if_t *common_if;
} mgc_display_lcd_if_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DISPLAY_LCD_IF_H*/
