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
    bool enabled;
    bool fontsize2x;
    int16_t x;
    int16_t y;
    mgc_rect_t bg_box;
    mgc_label_t item[MGC_SELECTBOX_MAX_SELECT_NUM];
    mgc_label_t cursor;
    size_t selected_idx;
    size_t item_count;
    uint8_t top_margin;
    uint8_t bottom_margin;
    uint8_t left_margin;
    uint8_t left_cursor_margin;
    uint8_t line_spacing;
    float parallax_factor_x;
    float parallax_factor_y;
} mgc_selectbox_t;

void selectbox_init(mgc_selectbox_t *selectbox, mgc_id_t id, const mgc_font_t *font, bool fontsize2x);
void selectbox_append_item(mgc_selectbox_t *selectbox, const char *text);
void selectbox_clear_items(mgc_selectbox_t *selectbox);
void selectbox_set_enabled(mgc_selectbox_t *selectbox, bool enabled);
void selectbox_set_position(mgc_selectbox_t *selectbox, int16_t x, int16_t y);
void selectbox_set_width(mgc_selectbox_t *selectbox, uint16_t width);
void selectbox_set_border_width(mgc_selectbox_t *selectbox, uint16_t width);
void selectbox_set_margin(mgc_selectbox_t *selectbox, uint8_t top, uint8_t bottom, uint8_t left);
void selectbox_set_cursor_margin(mgc_selectbox_t *selectbox, uint8_t left);
void selectbox_set_line_spacing(mgc_selectbox_t *selectbox, uint8_t line_spacing);
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

//////////////////////////////// Legacy ////////////////////////////////
bool selectbox_apply_cell_blending(const mgc_selectbox_t *selectbox, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void selectbox_set_r_cell_offset(mgc_selectbox_t *selectbox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SELECTBOX_H*/
