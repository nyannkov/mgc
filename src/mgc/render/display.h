/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DISPLAY_H
#define MGC_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef struct mgc_display_if {
    int (*init)(void);
    int (*deinit)(void);
    void (*reset)(void);
    int (*draw_rect)(uint8_t *buf, size_t len, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    uint16_t (*get_width)(void);
    uint16_t (*get_height)(void);
    void (*clear_screen)(void);
} mgc_display_if_t;

#ifdef __cplusplus
}/* extern "C" */
#endif
#endif/*MGC_DISPLAY_H*/
