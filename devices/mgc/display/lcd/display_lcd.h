/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DISPLAY_LCD_H
#define MGC_DISPLAY_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/display/interface/display_if.h"

extern const mgc_display_if_t * ( * const display_lcd_get_instance )(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DISPLAY_LCD_H*/
