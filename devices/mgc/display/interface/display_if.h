/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DISPLAY_DRIVER_H
#define MGC_DISPLAY_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef struct mgc_display_if {
    int (*init)(void);
    void (*reset)(void);
    int (*draw_rect)(uint8_t *buf, size_t len, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
} mgc_display_if_t;

#ifdef __cplusplus
}/* extern "C" */
#endif
#endif/*MGC_DISPLAY_DRIVER_H*/
