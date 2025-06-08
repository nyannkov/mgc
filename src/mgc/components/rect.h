/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_RECT_H
#define MGC_RECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/render/pixelbuffer.h"
#include "mgc/render/framebuffer.h"

typedef struct mgc_rect {
    mgc_id_t id;
    bool visible;
    int16_t x;
    int16_t y;
    float parallax_factor_x;
    float parallax_factor_y;
    uint16_t width;
    uint16_t height;
    uint16_t border_width;
    mgc_color_t inner_color;
    mgc_color_t border_color;
} mgc_rect_t;

void rect_init(mgc_rect_t *rect, mgc_id_t id);
void rect_set_id(mgc_rect_t *rect, mgc_id_t id);
void rect_set_visible(mgc_rect_t *rect, bool v);
void rect_set_position(mgc_rect_t *rect, int16_t x, int16_t y);
void rect_set_width(mgc_rect_t *rect, uint16_t width);
void rect_set_height(mgc_rect_t *rect, uint16_t height);
void rect_set_border_width(mgc_rect_t *rect, uint16_t border_width);
void rect_set_inner_color(mgc_rect_t *rect, mgc_color_t inner_color);
void rect_set_border_color(mgc_rect_t *rect, mgc_color_t border_color);
void rect_set_parallax_factor(mgc_rect_t *rect, float factor_x, float factor_y);
bool rect_draw(const mgc_rect_t *rect, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool rect_draw_cell(
        const mgc_rect_t *rect,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool rect_draw_raw(
        const mgc_rect_t *rect,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool rect_draw_cell_raw(
        const mgc_rect_t *rect,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

static inline
mgc_id_t rect_get_id(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    return rect->id;
}

static inline
bool rect_get_visible(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    return rect->visible;
}

static inline
mgc_point_t rect_get_position(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    mgc_point_t point = {rect->x, rect->y};
    return point;
}

static inline
mgc_size_t rect_get_size(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    mgc_size_t size = {rect->width, rect->height};
    return size;
}

static inline
mgc_parallax_factor_t rect_get_parallax_factor(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    mgc_parallax_factor_t factor = {
        rect->parallax_factor_x,
        rect->parallax_factor_y
    };
    return factor;
}

static inline
uint16_t rect_get_border_width(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    return rect->border_width;
}

static inline
mgc_color_t rect_get_inner_color(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    return rect->inner_color;
}

static inline
mgc_color_t rect_get_border_color(const mgc_rect_t *rect) {
    MGC_ASSERT(rect != NULL, "Invalid handler");
    return rect->border_color;
}

//////////////////////////////// Legacy ////////////////////////////////
bool rect_apply_cell_blending(const mgc_rect_t *rect, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void rect_set_r_cell_offset(mgc_rect_t *rect, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
#define rect_set_enabled    rect_set_visible

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_RECT_H*/
