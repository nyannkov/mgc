/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FRAMEBUFFER_H
#define MGC_FRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef struct mgc_framebuffer {
    mgc_color_t *buffer;
    uint16_t width;
    uint16_t height;
} mgc_framebuffer_t;

void framebuffer_init(mgc_framebuffer_t *fb, mgc_color_t *buffer, uint16_t width, uint16_t height);
void framebuffer_clear(mgc_framebuffer_t *fb, mgc_color_t color);

const mgc_color_t *framebuffer_get_buffer(const mgc_framebuffer_t *fb);
mgc_color_t *framebuffer_get_buffer_mut(mgc_framebuffer_t *fb);
uint16_t framebuffer_get_width(const mgc_framebuffer_t *fb);
uint16_t framebuffer_get_height(const mgc_framebuffer_t *fb);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_FRAMEBUFFER_H*/
