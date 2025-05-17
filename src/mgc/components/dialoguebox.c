/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "dialoguebox.h"

void dialoguebox_init(mgc_dialoguebox_t *dialoguebox, mgc_id_t id, const mgc_font_t *font, bool fontsize2x) {
    uint16_t text_width, text_height;
    if ( ( dialoguebox == NULL ) ||
         ( font == NULL ) ||
         ( font->fbb_x > MGC_FONT_MAX_FONT_SIZE ) ||
         ( font->fbb_y > MGC_FONT_MAX_FONT_SIZE )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    dialoguebox->top_margin = 8;
    dialoguebox->bottom_margin = 8;
    dialoguebox->left_margin = 8;
    dialoguebox->right_margin = 8;

    rect_init(&dialoguebox->bg_box, 0);
    rect_set_position(&dialoguebox->bg_box, 0, 0);
    rect_set_border_width(&dialoguebox->bg_box, 2);
    rect_set_width(&dialoguebox->bg_box, 16);
    rect_set_height(&dialoguebox->bg_box, 16);

    text_width = dialoguebox->bg_box.width - (dialoguebox->left_margin+dialoguebox->right_margin);
    text_height = dialoguebox->bg_box.height - (dialoguebox->top_margin+dialoguebox->bottom_margin);
    textblock_init(&dialoguebox->textblock, 0, font, fontsize2x);
    textblock_set_width(&dialoguebox->textblock, text_width);
    textblock_set_height(&dialoguebox->textblock, text_height);
    textblock_set_position(&dialoguebox->textblock, dialoguebox->left_margin, dialoguebox->top_margin);
    textblock_set_enable_back_color(&dialoguebox->textblock, false);

    rect_set_enabled(&dialoguebox->bg_box, true);
    textblock_set_enabled(&dialoguebox->textblock, true);
    dialoguebox->enabled = MGC_DEFAULT_ENABLED;
}

void dialoguebox_set_enabled(mgc_dialoguebox_t *dialoguebox, bool enabled) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    dialoguebox->enabled = enabled;
    rect_set_enabled(&dialoguebox->bg_box, enabled);
    textblock_set_enabled(&dialoguebox->textblock, enabled);
}

void dialoguebox_set_width(mgc_dialoguebox_t *dialoguebox, uint16_t width) {
    uint16_t text_width;
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( width < 16 ) {
        width = 16;
    }
    rect_set_width(&dialoguebox->bg_box, width);
    text_width = width - (dialoguebox->left_margin+dialoguebox->right_margin);
    textblock_set_width(&dialoguebox->textblock, text_width);
}

void dialoguebox_set_height(mgc_dialoguebox_t *dialoguebox, uint16_t height) {
    uint16_t text_height;
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( height < 16 ) {
        height = 16;
    }
    rect_set_height(&dialoguebox->bg_box, height);
    text_height = height - (dialoguebox->top_margin+dialoguebox->bottom_margin);
    textblock_set_height(&dialoguebox->textblock, text_height);
}

void dialoguebox_set_position(mgc_dialoguebox_t *dialoguebox, int16_t x, int16_t y) {
    int16_t text_x, text_y;
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_position(&dialoguebox->bg_box, x, y);
    text_x = x + dialoguebox->left_margin;
    text_y = y + dialoguebox->top_margin;
    textblock_set_position(&dialoguebox->textblock, text_x, text_y);
}

void dialoguebox_set_margin(mgc_dialoguebox_t *dialoguebox, uint8_t top, uint8_t bottom, uint8_t left, uint8_t right) {
    int16_t text_x, text_y;
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    dialoguebox->top_margin = top;
    dialoguebox->bottom_margin = bottom;
    dialoguebox->right_margin = right;
    dialoguebox->left_margin = left;

    textblock_set_width(&dialoguebox->textblock, dialoguebox->bg_box.width - ((uint16_t)left+right));
    textblock_set_width(&dialoguebox->textblock, dialoguebox->bg_box.height- ((uint16_t)top+bottom));
    textblock_set_position(&dialoguebox->textblock, dialoguebox->bg_box.x + left, dialoguebox->bg_box.y + top);
}

void dialoguebox_set_fore_color(mgc_dialoguebox_t *dialoguebox, mgc_color_t fore_color) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_fore_color(&dialoguebox->textblock, fore_color);
}

void dialoguebox_set_back_color(mgc_dialoguebox_t *dialoguebox, mgc_color_t back_color) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_back_color(&dialoguebox->textblock, back_color);
}

void dialoguebox_set_cursor_speed(mgc_dialoguebox_t *dialoguebox, uint8_t cursor_speed) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_cursor_speed(&dialoguebox->textblock, cursor_speed);
}

void dialoguebox_set_scroll_speed(mgc_dialoguebox_t *dialoguebox, uint8_t scroll_speed) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_scroll_speed(&dialoguebox->textblock, scroll_speed);
}

void dialoguebox_set_line_spacing(mgc_dialoguebox_t *dialoguebox, uint8_t line_spacing) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_line_spacing(&dialoguebox->textblock, line_spacing);
}

void dialoguebox_set_text(mgc_dialoguebox_t *dialoguebox, const char *text) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_text(&dialoguebox->textblock, text);
}

void dialoguebox_set_r_cell_offset(mgc_dialoguebox_t *dialoguebox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_r_cell_offset(&dialoguebox->bg_box, r_cell_x_ofs, r_cell_y_ofs);
    textblock_set_r_cell_offset(&dialoguebox->textblock, r_cell_x_ofs, r_cell_y_ofs);
}

void dialoguebox_set_scroll_line(mgc_dialoguebox_t *dialoguebox, uint8_t scroll_line) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_scroll_line(&dialoguebox->textblock, scroll_line);
}

void dialoguebox_adjust_height(mgc_dialoguebox_t *dialoguebox) {
    uint16_t height;
    uint16_t text_height;
    int16_t scale;
    if ( ( dialoguebox == NULL ) ||
         ( dialoguebox->textblock.font == NULL ) ||
         ( dialoguebox->textblock.scroll_line == 0 )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    text_height =
        ((uint16_t)dialoguebox->textblock.scroll_line - 1)
        * dialoguebox->textblock.line_spacing;
    scale = dialoguebox->textblock.fontsize2x ? 2 : 1;
    text_height +=
        (dialoguebox->textblock.font->fbb_y*scale)
        * dialoguebox->textblock.scroll_line;

    height = (uint16_t)dialoguebox->top_margin +
             (uint16_t)dialoguebox->bottom_margin +
             text_height;
    textblock_set_height(&dialoguebox->textblock, text_height);
    rect_set_height(&dialoguebox->bg_box, height);
}

void dialoguebox_display_update(mgc_dialoguebox_t *dialoguebox) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_display_update(&dialoguebox->textblock);
}

void dialoguebox_display_clear(mgc_dialoguebox_t *dialoguebox) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_display_clear(&dialoguebox->textblock);
}

enum mgc_display_text_state dialoguebox_get_display_text_state(const mgc_dialoguebox_t *dialoguebox) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return MGC_DISPLAY_TEXT_STATE_INIT;
    }
    return dialoguebox->textblock.state;
}

bool dialoguebox_apply_cell_blending(
    const mgc_dialoguebox_t *dialoguebox,
    mgc_pixelbuffer_t *pixelbuffer,
    int16_t cell_x,
    int16_t cell_y
) {
    bool is_blending = false;
    if ( ( dialoguebox == NULL ) ||
         ( pixelbuffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( dialoguebox->enabled == false ) {
        MGC_INFO("Handler is disabled");
        return false;
    }
    if ( rect_apply_cell_blending(&dialoguebox->bg_box, pixelbuffer, cell_x, cell_y) == true ) {
        is_blending = true;
    }
    if ( textblock_apply_cell_blending(&dialoguebox->textblock, pixelbuffer, cell_x, cell_y) == true ) {
        is_blending = true;
    }
    return is_blending;
}

bool dialoguebox_draw(
    const mgc_dialoguebox_t *dialoguebox,
    mgc_framebuffer_t *fb,
    const mgc_point_t *cam_pos,
    const mgc_draw_options_t *options
) {
    bool is_blending = false;
    if ( ( dialoguebox == NULL ) ||
         ( fb == NULL ) ||
         ( fb->buffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( dialoguebox->enabled == false ) {
        MGC_INFO("Handler is disabled");
        return false;
    }
    if ( rect_draw(&dialoguebox->bg_box, fb, cam_pos, options) == true ) {
        is_blending = true;
    }
    if ( textblock_draw(&dialoguebox->textblock, fb, cam_pos, options) == true ) {
        is_blending = true;
    }
    return is_blending;
}
