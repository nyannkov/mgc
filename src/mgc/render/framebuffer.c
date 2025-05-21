/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "framebuffer.h"


void framebuffer_init(mgc_framebuffer_t *fb, mgc_color_t *buffer, uint16_t width, uint16_t height) {
    if ( ( fb == NULL ) ||
         ( buffer == NULL ) ||
         ( width == 0 ) ||
         ( height == 0 )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    fb->buffer = buffer;
    fb->width = width;
    fb->height = height;
}

void framebuffer_clear(mgc_framebuffer_t *fb, mgc_color_t color) {
    mgc_color_t *dest;
    size_t len;

    if ( ( fb == NULL ) ||
         ( fb->buffer == NULL ) ||
         ( fb->width <= 0 ) ||
         ( fb->height <= 0 )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    dest = fb->buffer;
    len = (size_t)(fb->width) * (size_t)(fb->height);

    while ( len > 0 ) {
        *dest = MGC_COLOR_SWAP(color);
        dest++;
        len--;
    }
}

mgc_color_t *framebuffer_get_buffer(const mgc_framebuffer_t *fb) {
    if ( fb == NULL ) {
        MGC_WARN("Invalid handler");
        return NULL;
    }
    return fb->buffer;
}

uint16_t framebuffer_get_width(const mgc_framebuffer_t *fb) {
    if ( fb == NULL ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    return fb->width;
}

uint16_t framebuffer_get_height(const mgc_framebuffer_t *fb) {
    if ( fb == NULL ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    return fb->height;
}

