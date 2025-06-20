/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "selectbox.h"

#define MGC_SELECTBOX_DEFAULT_FBB_Y   (16)

static inline
void set_position_all(mgc_selectbox_t *selectbox, int16_t x, int16_t y) {

    selectbox->x = x;
    selectbox->y = y;

    rect_set_position(&selectbox->bg_box, x, y);

    for ( size_t idx = 0; idx < MGC_SELECTBOX_MAX_SELECT_NUM; idx++ ) {
        struct mgc_label *item = &selectbox->item[idx];
        int16_t x_item, y_item;
        int16_t item_height;
        item_height = ( selectbox->font != NULL ) ? item->font->fbb_y : MGC_SELECTBOX_DEFAULT_FBB_Y;
        if ( selectbox->fontsize2x == true ) {
            item_height *= 2;
        }
        x_item = x + selectbox->padding.left;
        y_item = y + selectbox->padding.top + (item_height + selectbox->line_spacing) * idx;
        label_set_position(item, x_item, y_item);
        if ( idx == selectbox->selected_idx ) {
            label_set_position(
                &selectbox->cursor,
                x+selectbox->left_cursor_margin,
                y_item
            );
        }
    }
}

void selectbox_init(mgc_selectbox_t *selectbox, mgc_id_t id, const mgc_font_t *font, bool fontsize2x) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->id = id;
    selectbox->x = 0;
    selectbox->y = 0;
    selectbox->fontsize2x = fontsize2x;
    selectbox->visible = MGC_DEFAULT_VISIBLE;
    selectbox->selected_idx = 0;
    selectbox->item_count = 0;

    selectbox->padding.top = 4;
    selectbox->padding.bottom = 4;
    selectbox->padding.left = 16;
    selectbox->padding.right = 8;

    selectbox->left_cursor_margin = 4;
    selectbox->line_spacing = 4;
    selectbox->parallax_factor_x = 0.0F;
    selectbox->parallax_factor_y = 0.0F;
    selectbox->font = font;

    rect_init(&selectbox->bg_box, 0);
    rect_set_visible(&selectbox->bg_box, true);
    rect_set_parallax_factor(&selectbox->bg_box, 0.0F, 0.0F);

    label_init(&selectbox->cursor, 0, font, fontsize2x);
    label_set_text(&selectbox->cursor, "*");
    label_resize_to_fit(&selectbox->cursor);
    label_set_visible(&selectbox->cursor, true);
    for (size_t i = 0; i < MGC_SELECTBOX_MAX_SELECT_NUM; i++ ) {
        label_init(&selectbox->item[i], 0, font, fontsize2x);
        label_set_visible(&selectbox->item[i], true);
    }

    rect_set_width(&selectbox->bg_box, 80);
    rect_set_border_width(&selectbox->bg_box, 2);
    rect_set_inner_color(&selectbox->bg_box, MGC_COLOR_BLACK);
    rect_set_border_color(&selectbox->bg_box, MGC_COLOR_WHITE);
}

void selectbox_set_id(mgc_selectbox_t *selectbox, mgc_id_t id) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->id = id;
}

void selectbox_set_font(mgc_selectbox_t *selectbox, const mgc_font_t *font) {
    if ( ( selectbox == NULL ) ||
         ( font == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->font = font;
    label_set_font(&selectbox->cursor, font);
    for (size_t i = 0; i < MGC_SELECTBOX_MAX_SELECT_NUM; i++ ) {
        label_set_font(&selectbox->item[i], font);
    }
}

void selectbox_set_fontsize2x(mgc_selectbox_t *selectbox, bool fontsize2x) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label_set_fontsize2x(&selectbox->cursor, fontsize2x);
    for (size_t i = 0; i < MGC_SELECTBOX_MAX_SELECT_NUM; i++ ) {
        label_set_fontsize2x(&selectbox->item[i], fontsize2x);
    }

    set_position_all(selectbox, selectbox->x, selectbox->y);
}

void selectbox_resize_to_fit(mgc_selectbox_t *selectbox) {
    uint16_t max_width = 0;
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    label_resize_to_fit(&selectbox->cursor);

    for ( size_t index = 0; index < selectbox->item_count; index++ ) {
        uint16_t item_width = 0;
        label_resize_to_fit(&selectbox->item[index]);
        item_width = selectbox->item[index].width;
        if ( max_width < item_width ) {
            max_width = item_width;
        }
    }
    rect_set_width(&selectbox->bg_box, max_width + (uint16_t)selectbox->padding.left + (uint16_t)selectbox->padding.right);
}

void selectbox_append_item(mgc_selectbox_t *selectbox, const char *text) {
    struct mgc_label *item;
    int16_t item_height;
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( selectbox->item_count >= MGC_SELECTBOX_MAX_SELECT_NUM ) {
        MGC_WARN("Full items");
        return;
    }
    item = &selectbox->item[selectbox->item_count];
    label_set_text(item, text);
    label_set_parallax_factor(
        item,
        selectbox->parallax_factor_x,
        selectbox->parallax_factor_y
    );
    selectbox->item_count++;

    item_height = ( selectbox->font != NULL ) ? item->font->fbb_y : MGC_SELECTBOX_DEFAULT_FBB_Y;
    if ( selectbox->fontsize2x == true ) {
        item_height *= 2;
    }
    rect_set_height(
        &selectbox->bg_box,
        selectbox->padding.top + selectbox->padding.bottom + item_height * selectbox->item_count +
        selectbox->line_spacing * (selectbox->item_count-1)
    );
}

void selectbox_clear_items(mgc_selectbox_t *selectbox) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->item_count = 0;
    selectbox->selected_idx = 0;
}

void selectbox_set_visible(mgc_selectbox_t *selectbox, bool v) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->visible = v;
}

void selectbox_set_position(mgc_selectbox_t *selectbox, int16_t x, int16_t y) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    set_position_all(selectbox, x, y);
}

void selectbox_set_width(mgc_selectbox_t *selectbox, uint16_t width) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_width(&selectbox->bg_box, width);
}

void selectbox_set_border_width(mgc_selectbox_t *selectbox, uint16_t width) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_border_width(&selectbox->bg_box, width);
}

void selectbox_set_padding(mgc_selectbox_t *selectbox, uint8_t top, uint8_t bottom, uint8_t left, uint8_t right) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->padding.top = top;
    selectbox->padding.bottom = bottom;
    selectbox->padding.left = left;
    selectbox->padding.right = right;
}

void selectbox_set_cursor_margin(mgc_selectbox_t *selectbox, uint8_t left) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->left_cursor_margin = left;
}

void selectbox_set_line_spacing(mgc_selectbox_t *selectbox, uint8_t line_spacing) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->line_spacing = line_spacing;
}

void selectbox_set_cursor_text(mgc_selectbox_t *selectbox, const char *text) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label_set_text(&selectbox->cursor, text);
}

void selectbox_set_selected_idx(mgc_selectbox_t *selectbox, size_t selected_idx) {
    struct mgc_label *item;
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( selectbox->item_count <= selected_idx ) {
        MGC_WARN("Out of index");
        return;
    }
    selectbox->selected_idx = selected_idx;
    item = &selectbox->item[selected_idx];
    label_set_position(
        &selectbox->cursor,
        selectbox->x+selectbox->left_cursor_margin,
        item->y
    );
}

void selectbox_change_selected_idx(mgc_selectbox_t *selectbox, bool increment) {
    size_t idx;
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( selectbox->item_count == 0 ) {
        MGC_WARN("Empty items");
        return;
    }
    idx = selectbox->selected_idx;
    if ( increment == true ) {
        if ( idx < selectbox->item_count - 1 ) {
            idx++;
        } else {
            idx = 0;
        }
    } else {
        if ( idx > 0 ) {
            idx--;
        } else {
            idx = selectbox->item_count - 1;
        }
    }
    selectbox_set_selected_idx(selectbox, idx);
}

void selectbox_set_box_inner_color(mgc_selectbox_t *selectbox, mgc_color_t color) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_inner_color(&selectbox->bg_box, color);
}

void selectbox_set_box_border_color(mgc_selectbox_t *selectbox, mgc_color_t color) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect_set_border_color(&selectbox->bg_box, color);
}

void selectbox_set_fore_color(mgc_selectbox_t *selectbox, mgc_color_t color) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label_set_fore_color(&selectbox->cursor, color);
    for ( size_t i = 0; i < selectbox->item_count; i++ ) {
        label_set_fore_color(&selectbox->item[i], color);
    }
}

void selectbox_set_parallax_factor(mgc_selectbox_t *selectbox, float factor_x, float factor_y) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->parallax_factor_x = factor_x;
    selectbox->parallax_factor_y = factor_y;
    rect_set_parallax_factor(&selectbox->bg_box, factor_x, factor_y);
    label_set_parallax_factor(&selectbox->cursor, factor_x, factor_y);
    for ( size_t i = 0; i < selectbox->item_count; i++ ) {
        label_set_parallax_factor(&selectbox->item[i], factor_x, factor_y);
    }
}

bool selectbox_draw(const mgc_selectbox_t *selectbox, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options) {

    if ( ( fb == NULL ) ||
         ( fb->buffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    return selectbox_draw_raw(selectbox, fb->buffer, fb->width, fb->height, cam_pos, options);
}

bool selectbox_draw_cell(
        const mgc_selectbox_t *selectbox,
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

    return selectbox_draw_cell_raw(selectbox, pb->pixelbuf, cell_x, cell_y, cam_pos, options);
}

bool selectbox_draw_raw(
        const mgc_selectbox_t *selectbox,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    bool is_blending = false;
    if ( ( selectbox == NULL ) ||
         ( buffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( selectbox->visible == false ) {
        MGC_INFO("Handler is not visible");
        return false;
    }
    if ( selectbox->item_count == 0 ) {
        MGC_WARN("Empty items");
        return false;
    }
    if ( rect_draw_raw(&selectbox->bg_box, buffer, width, height, cam_pos, options) == true ) {
        is_blending = true;
    }
    for ( size_t i = 0; i < selectbox->item_count; i++ ) {
        if ( label_draw_raw(&selectbox->item[i], buffer, width, height, cam_pos, options) == true ) {
            is_blending = true;
        }
    }
    if ( label_draw_raw(&selectbox->cursor, buffer, width, height, cam_pos, options) == true ) {
        is_blending = true;
    }

    return is_blending;
}

bool selectbox_draw_cell_raw(
        const mgc_selectbox_t *selectbox,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    if ( ( selectbox == NULL ) ||
         ( cell_buffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( selectbox->visible == false ) {
        MGC_INFO("Handler is not visible");
        return false;
    }
    if ( selectbox->item_count == 0 ) {
        MGC_WARN("Empty items");
        return false;
    }

    if ( rect_draw_cell_raw(&selectbox->bg_box, cell_buffer, cell_x, cell_y, cam_pos, options) == true ) {
        for ( size_t i = 0; i < selectbox->item_count; i++ ) {
            label_draw_cell_raw(&selectbox->item[i], cell_buffer, cell_x, cell_y, cam_pos, options);
        }
        label_draw_cell_raw(&selectbox->cursor, cell_buffer, cell_x, cell_y, cam_pos, options);
        return true;
    } else {
        return false;
    }
}

//////////////////////////////// Legacy ////////////////////////////////
bool selectbox_apply_cell_blending(const mgc_selectbox_t *selectbox, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    mgc_point_t cam_pos = {pixelbuffer->cell_x_ofs, pixelbuffer->cell_y_ofs};

    return selectbox_draw_cell(selectbox, pixelbuffer, cell_x, cell_y, &cam_pos, NULL);
}

void selectbox_set_r_cell_offset(mgc_selectbox_t *selectbox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->parallax_factor_x = (r_cell_x_ofs != 0) ? (1.0F / r_cell_x_ofs) : 0.0F;
    selectbox->parallax_factor_y = (r_cell_y_ofs != 0) ? (1.0F / r_cell_y_ofs) : 0.0F;
    rect_set_r_cell_offset(&selectbox->bg_box, r_cell_x_ofs, r_cell_y_ofs);
    label_set_r_cell_offset(&selectbox->cursor, r_cell_x_ofs, r_cell_y_ofs);
    for ( size_t i = 0; i < selectbox->item_count; i++ ) {
        label_set_r_cell_offset(&selectbox->item[i], r_cell_x_ofs, r_cell_y_ofs);
    }
}

