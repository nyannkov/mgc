/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SELECTBOX_H
#define MGC_SELECTBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/render/pixelbuffer.h"
#include "rect.h"
#include "label.h"

#ifndef MGC_SELECTBOX_MAX_SELECT_NUM
#define MGC_SELECTBOX_MAX_SELECT_NUM    6
#endif/*MGC_SELECTBOX_MAX_SELECT_NUM*/

typedef struct mgc_selectbox {
    mgc_id_t id;
    bool visible;
    bool fontsize2x;
    int16_t x;
    int16_t y;
    mgc_rect_t bg_box;
    mgc_label_t item[MGC_SELECTBOX_MAX_SELECT_NUM];
    mgc_label_t cursor;
    const mgc_font_t *font;
    size_t selected_idx;
    size_t item_count;
    mgc_padding_t padding;
    uint8_t left_cursor_margin;
    uint8_t item_spacing;
    float parallax_factor_x;
    float parallax_factor_y;
} mgc_selectbox_t;

void selectbox_init(mgc_selectbox_t *selectbox, mgc_id_t id, const mgc_font_t *font, bool fontsize2x);
void selectbox_set_id(mgc_selectbox_t *selectbox, mgc_id_t id);
void selectbox_set_font(mgc_selectbox_t *selectbox, const mgc_font_t *font);
void selectbox_set_fontsize2x(mgc_selectbox_t *selectbox, bool fontsize2x);
void selectbox_resize_to_fit(mgc_selectbox_t *selectbox);
void selectbox_append_item(mgc_selectbox_t *selectbox, const char *text);
void selectbox_clear_items(mgc_selectbox_t *selectbox);
void selectbox_set_visible(mgc_selectbox_t *selectbox, bool v);
void selectbox_set_position(mgc_selectbox_t *selectbox, int16_t x, int16_t y);
void selectbox_set_width(mgc_selectbox_t *selectbox, uint16_t width);
void selectbox_set_border_width(mgc_selectbox_t *selectbox, uint16_t width);
void selectbox_set_padding(mgc_selectbox_t *selectbox, uint8_t top, uint8_t bottom, uint8_t left, uint8_t right);
void selectbox_set_cursor_margin(mgc_selectbox_t *selectbox, uint8_t left);
void selectbox_set_item_spacing(mgc_selectbox_t *selectbox, uint8_t item_spacing);
void selectbox_set_cursor_text(mgc_selectbox_t *selectbox, const char *text);
void selectbox_set_selected_idx(mgc_selectbox_t *selectbox, size_t selected_idx);
void selectbox_change_selected_idx(mgc_selectbox_t *selectbox, bool increment);
void selectbox_set_box_inner_color(mgc_selectbox_t *selectbox, mgc_color_t color);
void selectbox_set_box_border_color(mgc_selectbox_t *selectbox, mgc_color_t color);
void selectbox_set_fore_color(mgc_selectbox_t *selectbox, mgc_color_t color);
void selectbox_set_parallax_factor(mgc_selectbox_t *selectbox, float factor_x, float factor_y);
bool selectbox_draw(const mgc_selectbox_t *selectbox, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool selectbox_draw_cell(
        const mgc_selectbox_t *selectbox,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool selectbox_draw_raw(
        const mgc_selectbox_t *selectbox,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool selectbox_draw_cell_raw(
        const mgc_selectbox_t *selectbox,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

static inline
mgc_id_t selectbox_get_id(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return selectbox->id;
}

static inline
const mgc_font_t* selectbox_get_font(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return label_get_font(&selectbox->item[0]);
}

static inline
bool selectbox_get_fontsize2x(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return label_get_fontsize2x(&selectbox->item[0]);
}

static inline
bool selectbox_get_visible(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return selectbox->visible;
}

static inline
mgc_point_t selectbox_get_position(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    mgc_point_t point = {selectbox->x, selectbox->y};
    return point;
}

static inline
mgc_size_t selectbox_get_size(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return rect_get_size(&selectbox->bg_box);
}

static inline
mgc_padding_t selectbox_get_padding(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return selectbox->padding;
}

static inline
mgc_parallax_factor_t selectbox_get_parallax_factor(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    mgc_parallax_factor_t factor = {
        selectbox->parallax_factor_x,
        selectbox->parallax_factor_y
    };
    return factor;
}

static inline
size_t selectbox_get_selected_idx(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return selectbox->selected_idx;
}

static inline
size_t selectbox_get_item_count(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return selectbox->item_count;
}

static inline
mgc_color_t selectbox_get_inner_color(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return rect_get_inner_color(&selectbox->bg_box);
}

static inline
mgc_color_t selectbox_get_border_color(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return rect_get_border_color(&selectbox->bg_box);
}

static inline
mgc_color_t selectbox_get_fore_color(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return label_get_fore_color(&selectbox->item[0]);
}

static inline
uint16_t selectbox_get_border_width(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return rect_get_border_width(&selectbox->bg_box);
}

static inline
const char *selectbox_get_cursor_text(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return label_get_text(&selectbox->cursor);
}

static inline
const char *selectbox_get_selected_item_text(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return label_get_text(&selectbox->item[selectbox->selected_idx]);
}

static inline
uint8_t selectbox_get_item_spacing(const mgc_selectbox_t *selectbox) {
    MGC_ASSERT(selectbox != NULL, "Invalid handler");
    return selectbox->item_spacing;
}

//////////////////////////////// Legacy ////////////////////////////////
bool selectbox_apply_cell_blending(const mgc_selectbox_t *selectbox, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void selectbox_set_r_cell_offset(mgc_selectbox_t *selectbox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
#define selectbox_set_enabled    selectbox_set_visible
#define selectbox_set_margin(handler, top, bottom, left)    selectbox_set_padding((handler), (top), (bottom), (left), 0)
#define selectbox_set_line_spacing     selectbox_set_item_spacing

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SELECTBOX_H*/
