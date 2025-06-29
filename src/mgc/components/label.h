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
    bool visible;
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
void label_set_id(mgc_label_t *label, mgc_id_t id);
void label_set_visible(mgc_label_t *label, bool v);
void label_set_position(mgc_label_t *label, int16_t x, int16_t y);
void label_set_size(mgc_label_t *label, uint16_t width, uint16_t height);
void label_set_width(mgc_label_t *label, uint16_t width);
void label_set_height(mgc_label_t *label, uint16_t height);
void label_set_parallax_factor(mgc_label_t *label, float factor_x, float factor_y);
void label_set_text(mgc_label_t *label, const char *text);
void label_set_fore_color(mgc_label_t *label, mgc_color_t fore_color);
void label_set_back_color(mgc_label_t *label, mgc_color_t back_color);
void label_set_enable_back_color(mgc_label_t *label, bool enable);
void label_set_font(mgc_label_t *label, const mgc_font_t *font);
void label_set_fontsize2x(mgc_label_t *label, bool fontsize2x);
void label_resize_to_fit(mgc_label_t *label);

bool label_draw(const mgc_label_t *label, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool label_draw_cell(
        const mgc_label_t *label,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool label_draw_raw(
        const mgc_label_t *label,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool label_draw_cell_raw(
        const mgc_label_t *label,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

static inline
mgc_id_t label_get_id(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->id;
}

static inline
const mgc_font_t* label_get_font(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->font;
}

static inline
bool label_get_fontsize2x(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->fontsize2x;
}

static inline
bool label_get_visible(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->visible;
}

static inline
mgc_point_t label_get_position(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    mgc_point_t point = {label->x, label->y};
    return point;
}

static inline
mgc_size_t label_get_size(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    mgc_size_t size = {label->width, label->height};
    return size;
}

static inline
mgc_parallax_factor_t label_get_parallax_factor(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    mgc_parallax_factor_t factor = {
        label->parallax_factor_x,
        label->parallax_factor_y
    };
    return factor;
}

static inline
const char* label_get_text(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->text;
}

static inline
mgc_color_t label_get_fore_color(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->fore_color;
}

static inline
mgc_color_t label_get_back_color(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->back_color;
}

static inline
bool label_get_enable_back_color(const mgc_label_t *label) {
    MGC_ASSERT(label != NULL, "Invalid handler");
    return label->enable_back_color;
}

//////////////////////////////// Legacy ////////////////////////////////
bool label_apply_cell_blending(const mgc_label_t *label, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void label_set_r_cell_offset(mgc_label_t *label, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
#define label_set_enabled    label_set_visible


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_LABEL_H*/
