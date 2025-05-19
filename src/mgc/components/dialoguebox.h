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
    bool enabled;
    mgc_rect_t bg_box;
    mgc_textblock_t textblock;
    uint8_t top_margin;
    uint8_t bottom_margin;
    uint8_t right_margin;
    uint8_t left_margin;
} mgc_dialoguebox_t;

void dialoguebox_init(mgc_dialoguebox_t *dialoguebox, mgc_id_t id, const mgc_font_t *font, bool fontsize2x);
void dialoguebox_set_enabled(mgc_dialoguebox_t *dialoguebox, bool enabled);
void dialoguebox_set_position(mgc_dialoguebox_t *dialoguebox, int16_t x, int16_t y);
void dialoguebox_set_width(mgc_dialoguebox_t *dialoguebox, uint16_t width);
void dialoguebox_set_height(mgc_dialoguebox_t *dialoguebox, uint16_t height);
void dialoguebox_set_margin(mgc_dialoguebox_t *dialoguebox, uint8_t top, uint8_t bottom, uint8_t left, uint8_t right);
void dialoguebox_set_fore_color(mgc_dialoguebox_t *dialoguebox, mgc_color_t fore_color);
void dialoguebox_set_back_color(mgc_dialoguebox_t *dialoguebox, mgc_color_t back_color);
void dialoguebox_set_cursor_speed(mgc_dialoguebox_t *dialoguebox, uint8_t cursor_speed);
void dialoguebox_set_scroll_speed(mgc_dialoguebox_t *dialoguebox, uint8_t scroll_speed);
void dialoguebox_set_line_spacing(mgc_dialoguebox_t *dialoguebox, uint8_t line_spacing);
void dialoguebox_set_r_cell_offset(mgc_dialoguebox_t *dialoguebox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
void dialoguebox_set_text(mgc_dialoguebox_t *dialoguebox, const char *text);
void dialoguebox_set_scroll_line(mgc_dialoguebox_t *dialoguebox, uint8_t scroll_line);
void dialoguebox_adjust_height(mgc_dialoguebox_t *dialoguebox);
void dialoguebox_display_update(mgc_dialoguebox_t *dialoguebox);
void dialoguebox_display_clear(mgc_dialoguebox_t *dialoguebox);
enum mgc_display_text_state dialoguebox_get_display_text_state(const mgc_dialoguebox_t *dialoguebox);
bool dialoguebox_draw(const mgc_dialoguebox_t *dialoguebox, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool dialoguebox_draw_cell(
        const mgc_dialoguebox_t *dialoguebox,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

// Legacy
bool dialoguebox_apply_cell_blending(const mgc_dialoguebox_t *dialoguebox, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DIALOGUEBOX_H*/
