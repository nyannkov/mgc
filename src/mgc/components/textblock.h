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
    uint8_t r_cell_x_ofs;
    uint8_t r_cell_y_ofs;
    int16_t width;
    int16_t height;
    const char *text;
    const mgc_font_t *font;
    mgc_color_t fore_color;
    mgc_color_t back_color;
    bool enable_back_color;
    bool fontsize2x;
    bool enabled;
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
void textblock_set_enabled(mgc_textblock_t *textblock, bool enabled);
void textblock_set_position(mgc_textblock_t *textblock, int16_t x, int16_t y);
void textblock_set_text(mgc_textblock_t *textblock, const char *text);
void textblock_set_height(mgc_textblock_t *textblock, uint16_t height);
void textblock_set_width(mgc_textblock_t *textblock, uint16_t width);
void textblock_set_fore_color(mgc_textblock_t *textblock, mgc_color_t fore_color);
void textblock_set_back_color(mgc_textblock_t *textblock, mgc_color_t back_color);
void textblock_set_enable_back_color(mgc_textblock_t *textblock, bool enable);
void textblock_set_cursor_speed(mgc_textblock_t *textblock, uint8_t cursor_speed);
void textblock_set_scroll_speed(mgc_textblock_t *textblock, uint8_t scroll_speed);
void textblock_set_line_spacing(mgc_textblock_t *textblock, uint8_t line_spacing);
void textblock_set_r_cell_offset(mgc_textblock_t *textblock, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
void textblock_set_scroll_line( mgc_textblock_t *textblock, uint8_t scroll_line);
void textblock_display_update(mgc_textblock_t *textblock);
void textblock_display_clear(mgc_textblock_t *textblock);
enum mgc_display_text_state textblock_get_display_text_state(const mgc_textblock_t *textblock);
bool textblock_apply_cell_blending(const mgc_textblock_t *textblock, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_TEXTBLOCK_H*/
