/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_LABEL_H
#define MGC_LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/font/font.h"
#include "mgc/font/encoding.h"
#include "mgc/render/pixelbuffer.h"
#include "mgc/render/framebuffer.h"

typedef struct mgc_label {
    mgc_id_t id;
    int16_t x;
    int16_t y;
    uint16_t width;
    uint16_t height;
    bool enabled;
    float parallax_factor_x;
    float parallax_factor_y;
    const char *text;
    const mgc_font_t *font;
    mgc_color_t fore_color;
    mgc_color_t back_color;
    bool fontsize2x;
    bool enable_back_color;
} mgc_label_t;

void label_init(mgc_label_t *label, mgc_id_t id, const mgc_font_t *font, bool fontsize2x);
void label_set_enabled(mgc_label_t *label, bool enabled);
void label_set_position(mgc_label_t *label, int16_t x, int16_t y);
void label_set_size(mgc_label_t *label, uint16_t width, uint16_t height);
void label_set_parallax_factor(mgc_label_t *label, float factor_x, float factor_y);
void label_set_text(mgc_label_t *label, const char *text);
void label_set_fore_color(mgc_label_t *label, mgc_color_t fore_color);
void label_set_back_color(mgc_label_t *label, mgc_color_t back_color);
void label_set_enable_back_color(mgc_label_t *label, bool enable);
bool label_draw(const mgc_label_t *label, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool label_draw_cell(
        const mgc_label_t *label,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

//////////////////////////////// Legacy ////////////////////////////////
bool label_apply_cell_blending(const mgc_label_t *label, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void label_set_r_cell_offset(mgc_label_t *label, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_LABEL_H*/
