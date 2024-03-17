/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "selectbox.h"

static void setup_form(mgc_selectbox_t *selectbox) {
    int16_t x, y;
    size_t m;
    x = selectbox->x;
    y = selectbox->y;
    m = (selectbox->fontsize2x == true) ? 2 : 1;

    rect_set_position(&selectbox->bg_box, x, y);
    for ( size_t idx = 0; idx < MGC_SELECTBOX_MAX_SELECT_NUM; idx++ ) {
        struct mgc_label *item = &selectbox->item[idx];
        int16_t x_item, y_item;
        x_item = x + selectbox->left_margin;
        y_item = y + selectbox->top_margin + 
            (item->font->fbb_y*m + selectbox->line_spacing) * idx;
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
    if ( ( selectbox == NULL ) ||
         ( font == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->x = 0;
    selectbox->y = 0;
    selectbox->fontsize2x = fontsize2x;
    selectbox->enabled = MGC_DEFAULT_ENABLED;
    selectbox->selected_idx = 0;
    selectbox->item_count = 0;
    selectbox->top_margin = 4;
    selectbox->bottom_margin = 4;
    selectbox->left_margin = 16;
    selectbox->left_cursor_margin = 4;
    selectbox->line_spacing = 4;
    selectbox->r_cell_x_ofs = 1;
    selectbox->r_cell_y_ofs = 1;

    rect_init(&selectbox->bg_box, 0);
    rect_set_enabled(&selectbox->bg_box, true);

    label_init(&selectbox->cursor, 0, font, fontsize2x);
    label_set_text(&selectbox->cursor, "*");
    label_set_enabled(&selectbox->cursor, true);
    for (size_t i = 0; i < MGC_SELECTBOX_MAX_SELECT_NUM; i++ ) {
        label_init(&selectbox->item[i], 0, font, fontsize2x);
        label_set_enabled(&selectbox->item[i], true);
    }

    rect_set_width(&selectbox->bg_box, 80);
    rect_set_border_width(&selectbox->bg_box, 2);
    rect_set_inner_color(&selectbox->bg_box, MGC_COLOR_BLACK);
    rect_set_border_color(&selectbox->bg_box, MGC_COLOR_WHITE);

    setup_form(selectbox);
}

void selectbox_append_item(mgc_selectbox_t *selectbox, const char *text) {
    struct mgc_label *item;
    uint16_t box_width;
    size_t m;
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
    label_set_r_cell_offset(
        item,
        selectbox->r_cell_x_ofs,
        selectbox->r_cell_y_ofs
    );
    selectbox->item_count++;

    m = (selectbox->fontsize2x == true) ? 2 : 1;
    rect_set_height(
        &selectbox->bg_box,
        selectbox->top_margin + selectbox->bottom_margin +
        (item->font->fbb_y*m) * selectbox->item_count +
        selectbox->line_spacing * (selectbox->item_count-1)
    );
}

void selectbox_clear_items(mgc_selectbox_t *selectbox) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->item_count = 0;
}

void selectbox_set_enabled(mgc_selectbox_t *selectbox, bool enabled) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->enabled = enabled;
}

void selectbox_set_position(mgc_selectbox_t *selectbox, int16_t x, int16_t y) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->x = x;
    selectbox->y = y;
    setup_form(selectbox);
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

void selectbox_set_margin(mgc_selectbox_t *selectbox, uint8_t top, uint8_t bottom, uint8_t left) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->top_margin = top;
    selectbox->bottom_margin = bottom;
    selectbox->left_margin = left;
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

void selectbox_set_r_cell_offset(mgc_selectbox_t *selectbox, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( selectbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    selectbox->r_cell_x_ofs = r_cell_x_ofs;
    selectbox->r_cell_y_ofs = r_cell_y_ofs;
    rect_set_r_cell_offset(&selectbox->bg_box, r_cell_x_ofs, r_cell_y_ofs);
    label_set_r_cell_offset(&selectbox->cursor, r_cell_x_ofs, r_cell_y_ofs);
    for ( size_t i = 0; i < selectbox->item_count; i++ ) {
        label_set_r_cell_offset(&selectbox->item[i], r_cell_x_ofs, r_cell_y_ofs);
    }
}

bool selectbox_apply_cell_blending(const mgc_selectbox_t *selectbox, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {
    bool is_blending = false;
    if ( ( selectbox == NULL ) ||
         ( pixelbuffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( selectbox->enabled == false ) {
        MGC_INFO("Handler is disabled");
        return false;
    }
    if ( selectbox->item_count == 0 ) {
        MGC_WARN("Empty items");
        return false;
    }
    if ( rect_apply_cell_blending(&selectbox->bg_box, pixelbuffer, cell_x, cell_y) == true ) {
        is_blending = true;
    }
    for ( size_t i = 0; i < selectbox->item_count; i++ ) {
        if ( label_apply_cell_blending(&selectbox->item[i], pixelbuffer, cell_x, cell_y) == true ) {
            is_blending = true;
        }
    }
    if ( label_apply_cell_blending(&selectbox->cursor, pixelbuffer, cell_x, cell_y) == true ) {
        is_blending = true;
    }

    return is_blending;
}

