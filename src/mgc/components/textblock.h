/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_TEXTBLOCK_H
#define MGC_TEXTBLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/font/font.h"
#include "mgc/font/encoding.h"
#include "mgc/render/pixelbuffer.h"
#include "mgc/render/framebuffer.h"

#ifndef MGC_TEXTBLOCK_MAX_LINES
#define MGC_TEXTBLOCK_MAX_LINES         (16)
#endif/*MGC_TEXTBLOCK_MAX_LINES*/

enum mgc_display_text_state {
    MGC_DISPLAY_TEXT_STATE_INIT,
    MGC_DISPLAY_TEXT_STATE_CURSOR_MOVING,
    MGC_DISPLAY_TEXT_STATE_TEXT_SCROLLING,
    MGC_DISPLAY_TEXT_STATE_TEXT_END
};

typedef struct mgc_textblock {
    mgc_id_t id;
    int16_t x;
    int16_t y;
    float parallax_factor_x;
    float parallax_factor_y;
    uint16_t width;
    uint16_t height;
    const char *text;
    const mgc_font_t *font;
    mgc_color_t fore_color;
    mgc_color_t back_color;
    bool enable_back_color;
    bool fontsize2x;
    bool visible;
    const char *lines[MGC_TEXTBLOCK_MAX_LINES];
    const char *cursor;
    uint8_t line_count;
    uint8_t line_ofs;
    uint8_t current_line;
    uint8_t cursor_speed;
    uint8_t scroll_speed;
    uint8_t scroll_line;
    uint8_t scroll_y;
    uint8_t line_spacing;
    enum mgc_display_text_state state;
} mgc_textblock_t;

void textblock_init(mgc_textblock_t *textblock, mgc_id_t id, const mgc_font_t *font, bool fontsize2x);
void textblock_set_id(mgc_textblock_t *textblock, mgc_id_t id);
void textblock_set_visible(mgc_textblock_t *textblock, bool v);
void textblock_set_position(mgc_textblock_t *textblock, int16_t x, int16_t y);
void textblock_set_text(mgc_textblock_t *textblock, const char *text);
void textblock_set_height(mgc_textblock_t *textblock, uint16_t height);
void textblock_set_width(mgc_textblock_t *textblock, uint16_t width);
void textblock_set_font(mgc_textblock_t *textblock, const mgc_font_t *font);
void textblock_set_fontsize2x(mgc_textblock_t *textblock, bool fontsize2x);
void textblock_set_fore_color(mgc_textblock_t *textblock, mgc_color_t fore_color);
void textblock_set_back_color(mgc_textblock_t *textblock, mgc_color_t back_color);
void textblock_set_enable_back_color(mgc_textblock_t *textblock, bool enable);
void textblock_set_cursor_speed(mgc_textblock_t *textblock, uint8_t cursor_speed);
void textblock_set_scroll_speed(mgc_textblock_t *textblock, uint8_t scroll_speed);
void textblock_set_line_spacing(mgc_textblock_t *textblock, uint8_t line_spacing);
void textblock_set_parallax_factor(mgc_textblock_t *textblock, float factor_x, float factor_y);
void textblock_set_scroll_line(mgc_textblock_t *textblock, uint8_t scroll_line);
void textblock_display_update(mgc_textblock_t *textblock);
void textblock_display_clear(mgc_textblock_t *textblock);
void textblock_set_font(mgc_textblock_t *textblock, const mgc_font_t *font);
void textblock_set_fontsize2x(mgc_textblock_t *textblock, bool fontsize2x);
bool textblock_draw(const mgc_textblock_t *textblock, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool textblock_draw_cell(
        const mgc_textblock_t *textblock,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool textblock_draw_raw(
        const mgc_textblock_t *textblock,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool textblock_draw_cell_raw(
        const mgc_textblock_t *textblock,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

static inline
enum mgc_display_text_state textblock_get_display_text_state(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->state;
}

static inline
mgc_id_t textblock_get_id(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->id;
}

static inline
const mgc_font_t* textblock_get_font(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->font;
}

static inline
bool textblock_get_fontsize2x(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->fontsize2x;
}

static inline
bool textblock_get_visible(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->visible;
}

static inline
mgc_point_t textblock_get_position(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    mgc_point_t point = {textblock->x, textblock->y};
    return point;
}

static inline
mgc_size_t textblock_get_size(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    mgc_size_t size = {textblock->width, textblock->height};
    return size;
}

static inline
mgc_parallax_factor_t textblock_get_parallax_factor(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    mgc_parallax_factor_t factor = {
        textblock->parallax_factor_x,
        textblock->parallax_factor_y
    };
    return factor;
}

static inline
const char* textblock_get_text(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->text;
}

static inline
mgc_color_t textblock_get_fore_color(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->fore_color;
}

static inline
mgc_color_t textblock_get_back_color(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->back_color;
}

static inline
bool textblock_get_enable_back_color(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->enable_back_color;
}

static inline
uint8_t textblock_get_cursor_speed(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->cursor_speed;
}

static inline
uint8_t textblock_get_scroll_speed(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->scroll_speed;
}

static inline
uint8_t textblock_get_line_spacing(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->line_spacing;
}

static inline
uint8_t textblock_get_scroll_line(const mgc_textblock_t *textblock) {
    MGC_ASSERT(textblock != NULL, "Invalid handler");
    return textblock->scroll_line;
}

//////////////////////////////// Legacy ////////////////////////////////
bool textblock_apply_cell_blending(const mgc_textblock_t *textblock, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void textblock_set_r_cell_offset(mgc_textblock_t *textblock, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
#define textblock_set_enabled    textblock_set_visible

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_TEXTBLOCK_H*/
