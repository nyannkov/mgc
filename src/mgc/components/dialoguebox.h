/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DIALOGUEBOX_H
#define MGC_DIALOGUEBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "textblock.h"
#include "rect.h"

typedef struct mgc_dialoguebox {
    mgc_id_t id;
    bool visible;
    mgc_rect_t bg_box;
    mgc_textblock_t textblock;
    uint8_t top_margin;
    uint8_t bottom_margin;
    uint8_t right_margin;
    uint8_t left_margin;
} mgc_dialoguebox_t;

void dialoguebox_init(mgc_dialoguebox_t *dialoguebox, mgc_id_t id, const mgc_font_t *font, bool fontsize2x);
void dialoguebox_set_id(mgc_dialoguebox_t *dialoguebox, mgc_id_t id);
void dialoguebox_set_visible(mgc_dialoguebox_t *dialoguebox, bool v);
void dialoguebox_set_position(mgc_dialoguebox_t *dialoguebox, int16_t x, int16_t y);
void dialoguebox_set_width(mgc_dialoguebox_t *dialoguebox, uint16_t width);
void dialoguebox_set_height(mgc_dialoguebox_t *dialoguebox, uint16_t height);
void dialoguebox_set_margin(mgc_dialoguebox_t *dialoguebox, uint8_t top, uint8_t bottom, uint8_t left, uint8_t right);
void dialoguebox_set_fore_color(mgc_dialoguebox_t *dialoguebox, mgc_color_t fore_color);
void dialoguebox_set_back_color(mgc_dialoguebox_t *dialoguebox, mgc_color_t back_color);
void dialoguebox_set_cursor_speed(mgc_dialoguebox_t *dialoguebox, uint8_t cursor_speed);
void dialoguebox_set_scroll_speed(mgc_dialoguebox_t *dialoguebox, uint8_t scroll_speed);
void dialoguebox_set_line_spacing(mgc_dialoguebox_t *dialoguebox, uint8_t line_spacing);
void dialoguebox_set_parallax_factor(mgc_dialoguebox_t *dialoguebox, float factor_x, float factor_y);
void dialoguebox_set_text(mgc_dialoguebox_t *dialoguebox, const char *text);
void dialoguebox_set_font(mgc_dialoguebox_t *dialoguebox, const mgc_font_t *font);
void dialoguebox_set_fontsize2x(mgc_dialoguebox_t *dialoguebox, bool fontsize2x);
void dialoguebox_set_scroll_line(mgc_dialoguebox_t *dialoguebox, uint8_t scroll_line);
void dialoguebox_adjust_height(mgc_dialoguebox_t *dialoguebox);
void dialoguebox_display_update(mgc_dialoguebox_t *dialoguebox);
void dialoguebox_display_clear(mgc_dialoguebox_t *dialoguebox);
bool dialoguebox_draw(const mgc_dialoguebox_t *dialoguebox, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool dialoguebox_draw_cell(
        const mgc_dialoguebox_t *dialoguebox,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool dialoguebox_draw_raw(
        const mgc_dialoguebox_t *dialoguebox,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool dialoguebox_draw_cell_raw(
        const mgc_dialoguebox_t *dialoguebox,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

static inline
enum mgc_display_text_state dialoguebox_get_display_text_state(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_display_text_state(&dialoguebox->textblock);
}

static inline
mgc_id_t dialoguebox_get_id(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return dialoguebox->id;
}

static inline
const mgc_font_t* dialoguebox_get_font(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_font(&dialoguebox->textblock);
}

static inline
bool dialoguebox_get_fontsize2x(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_fontsize2x(&dialoguebox->textblock);
}

static inline
bool dialoguebox_get_visible(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return dialoguebox->visible;
}

static inline
mgc_point_t dialoguebox_get_position(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return rect_get_position(&dialoguebox->bg_box);
}

static inline
mgc_size_t dialoguebox_get_size(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return rect_get_size(&dialoguebox->bg_box);
}

static inline
mgc_parallax_factor_t dialoguebox_get_parallax_factor(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return rect_get_parallax_factor(&dialoguebox->bg_box);
}

static inline
const char* dialoguebox_get_text(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_text(&dialoguebox->textblock);
}

static inline
mgc_color_t dialoguebox_get_fore_color(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_fore_color(&dialoguebox->textblock);
}

static inline
mgc_color_t dialoguebox_get_back_color(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_back_color(&dialoguebox->textblock);
}

static inline
bool dialoguebox_get_enable_back_color(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_enable_back_color(&dialoguebox->textblock);
}

static inline
uint8_t dialoguebox_get_cursor_speed(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_cursor_speed(&dialoguebox->textblock);
}

static inline
uint8_t dialoguebox_get_scroll_speed(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_scroll_speed(&dialoguebox->textblock);
}

static inline
uint8_t dialoguebox_get_line_spacing(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_line_spacing(&dialoguebox->textblock);
}

static inline
uint8_t dialoguebox_get_scroll_line(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    return textblock_get_scroll_line(&dialoguebox->textblock);
}

static inline
mgc_margin_region_t dialoguebox_get_margin(const mgc_dialoguebox_t *dialoguebox) {
    MGC_ASSERT(dialoguebox != NULL, "Invalid handler");
    mgc_margin_region_t region = {
        dialoguebox->left_margin,
        dialoguebox->right_margin,
        dialoguebox->top_margin,
        dialoguebox->bottom_margin
    };
    return region;
}

//////////////////////////////// Legacy ////////////////////////////////
bool dialoguebox_apply_cell_blending(const mgc_dialoguebox_t *dialoguebox, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void dialoguebox_set_r_cell_offset(mgc_dialoguebox_t *dialoguebox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
#define dialoguebox_set_enabled    dialoguebox_set_visible

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DIALOGUEBOX_H*/
