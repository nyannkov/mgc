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
    bool enabled;
    int16_t x;
    int16_t y;
    uint8_t parallax_factor_x;
    uint8_t parallax_factor_y;
    uint16_t width;
    uint16_t height;
    uint16_t border_width;
    mgc_color_t inner_color;
    mgc_color_t border_color;
} mgc_rect_t;

void rect_init(mgc_rect_t *rect, mgc_id_t id);
void rect_set_enabled(mgc_rect_t *rect, bool enabled);
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

//////////////////////////////// Legacy ////////////////////////////////
bool rect_apply_cell_blending(const mgc_rect_t *rect, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void rect_set_r_cell_offset(mgc_rect_t *rect, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_RECT_H*/
