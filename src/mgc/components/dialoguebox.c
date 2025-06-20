/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "dialoguebox.h"

void dialoguebox_init(mgc_dialoguebox_t *dialoguebox, mgc_id_t id, const mgc_font_t *font, bool fontsize2x) {
    uint16_t text_width, text_height;
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    dialoguebox->id = id;

    dialoguebox->top_margin = 8;
    dialoguebox->bottom_margin = 8;
    dialoguebox->left_margin = 8;
    dialoguebox->right_margin = 8;

    rect_init(&dialoguebox->bg_box, 0);
    rect_set_position(&dialoguebox->bg_box, 0, 0);
    rect_set_border_width(&dialoguebox->bg_box, 2);
    rect_set_width(&dialoguebox->bg_box, 16);
    rect_set_height(&dialoguebox->bg_box, 16);
    rect_set_parallax_factor(&dialoguebox->bg_box, 0.0F, 0.0F);

    text_width = dialoguebox->bg_box.width - (dialoguebox->left_margin+dialoguebox->right_margin);
    text_height = dialoguebox->bg_box.height - (dialoguebox->top_margin+dialoguebox->bottom_margin);
    textblock_init(&dialoguebox->textblock, 0, font, fontsize2x);
    textblock_set_width(&dialoguebox->textblock, text_width);
    textblock_set_height(&dialoguebox->textblock, text_height);
    textblock_set_position(&dialoguebox->textblock, dialoguebox->left_margin, dialoguebox->top_margin);
    textblock_set_enable_back_color(&dialoguebox->textblock, false);
    textblock_set_parallax_factor(&dialoguebox->textblock, 0.0F, 0.0F);

    rect_set_visible(&dialoguebox->bg_box, true);
    textblock_set_visible(&dialoguebox->textblock, true);
    dialoguebox->visible = MGC_DEFAULT_VISIBLE;
}

void dialoguebox_set_id(mgc_dialoguebox_t *dialoguebox, mgc_id_t id) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    dialoguebox->id = id;
}

void dialoguebox_set_visible(mgc_dialoguebox_t *dialoguebox, bool v) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    dialoguebox->visible = v;
    rect_set_visible(&dialoguebox->bg_box, v);
    textblock_set_visible(&dialoguebox->textblock, v);
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

void dialoguebox_set_font(mgc_dialoguebox_t *dialoguebox, const mgc_font_t *font) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_font(&dialoguebox->textblock, font);
}

void dialoguebox_set_fontsize2x(mgc_dialoguebox_t *dialoguebox, bool fontsize2x) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_fontsize2x(&dialoguebox->textblock, fontsize2x);
}

void dialoguebox_set_parallax_factor(mgc_dialoguebox_t *dialoguebox, float factor_x, float factor_y) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_parallax_factor(&dialoguebox->bg_box, factor_x, factor_y);
    textblock_set_parallax_factor(&dialoguebox->textblock, factor_x, factor_y);
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

bool dialoguebox_draw(
    const mgc_dialoguebox_t *dialoguebox,
    mgc_framebuffer_t *fb,
    const mgc_point_t *cam_pos,
    const mgc_draw_options_t *options
) {
    if ( ( fb == NULL ) ||
         ( fb->buffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    return dialoguebox_draw_raw(dialoguebox, fb->buffer, fb->width, fb->height, cam_pos, options);
}

bool dialoguebox_draw_cell(
        const mgc_dialoguebox_t *dialoguebox,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {

    if ( pb == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    return dialoguebox_draw_cell_raw(dialoguebox, pb->pixelbuf, cell_x, cell_y, cam_pos, options);
}

bool dialoguebox_draw_raw(
        const mgc_dialoguebox_t *dialoguebox,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    bool is_blending = false;
    if ( ( dialoguebox == NULL ) ||
         ( buffer == NULL ) 
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( dialoguebox->visible == false ) {
        MGC_INFO("Handler is not visible");
        return false;
    }
    if ( rect_draw_raw(&dialoguebox->bg_box, buffer, width, height, cam_pos, options) == true ) {
        is_blending = true;
    }
    if ( textblock_draw_raw(&dialoguebox->textblock, buffer, width, height, cam_pos, options) == true ) {
        is_blending = true;
    }
    return is_blending;
}

bool dialoguebox_draw_cell_raw(
        const mgc_dialoguebox_t *dialoguebox,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    if ( ( dialoguebox == NULL ) ||
         ( cell_buffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( dialoguebox->visible == false ) {
        MGC_INFO("Handler is not visible");
        return false;
    }

    if ( rect_draw_cell_raw(&dialoguebox->bg_box, cell_buffer, cell_x, cell_y, cam_pos, options) == true ) {
        textblock_draw_cell_raw(&dialoguebox->textblock, cell_buffer, cell_x, cell_y, cam_pos, options);
        return true;
    } else {
        return false;
    }
}

//////////////////////////////// Legacy ////////////////////////////////
bool dialoguebox_apply_cell_blending(
    const mgc_dialoguebox_t *dialoguebox,
    mgc_pixelbuffer_t *pixelbuffer,
    int16_t cell_x,
    int16_t cell_y
) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    mgc_point_t cam_pos = {pixelbuffer->cell_x_ofs, pixelbuffer->cell_y_ofs};

    return dialoguebox_draw_cell(dialoguebox, pixelbuffer, cell_x, cell_y, &cam_pos, NULL);
}

void dialoguebox_set_r_cell_offset(mgc_dialoguebox_t *dialoguebox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( dialoguebox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_r_cell_offset(&dialoguebox->bg_box, r_cell_x_ofs, r_cell_y_ofs);
    textblock_set_r_cell_offset(&dialoguebox->textblock, r_cell_x_ofs, r_cell_y_ofs);
}


