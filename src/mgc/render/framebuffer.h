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

static inline 
void framebuffer_draw_pixel(mgc_framebuffer_t *fb, uint16_t x, uint16_t y, mgc_color_t color) {
    if ( ( fb == NULL ) ||
         ( fb->buffer == NULL ) ||
         ( fb->width <= 0 ) ||
         ( fb->height <= 0 )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    if ( ( x >= fb->width ) || ( y >= fb->height ) ) {
        MGC_INFO("Out-of-bounds pixel skipped");
    }

    fb->buffer[(size_t)x + (size_t)y * (size_t)(fb->width)] = MGC_COLOR_SWAP(color);
}


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_FRAMEBUFFER_H*/
