/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "rect.h"

typedef struct draw_box {
    int16_t right;
    int16_t left;
    int16_t top;
    int16_t bottom;
} draw_box_t;

static inline bool apply_cell_blending(mgc_color_t *pixelbuf, const draw_box_t *target, const draw_box_t *cell, mgc_color_t color) {
    if ( (target->left<=cell->right) && (cell->left<=target->right) && (target->top<=cell->bottom) && (cell->top<=target->bottom) ) {
        int16_t x, y;
        int16_t x_s, y_s, x_e, y_e;
        x_s = (( cell->left   < target->left   ) ? target->left : cell->left     ) - target->left;
        x_e = (( cell->right  < target->right  ) ? cell->right  : target->right  ) - target->left;
        y_s = (( cell->top    < target->top    ) ? target->top  : cell->top      ) - target->top;
        y_e = (( cell->bottom < target->bottom ) ? cell->bottom : target->bottom ) - target->top;
        for ( x = x_s; x <= x_e; x++ ) {
            for ( y = y_s; y <= y_e; y++ ) {
                size_t idx;
                idx = MGC_GET_PIXELBUF_INDEX(x+target->left-cell->left, y+target->top-cell->top);
                pixelbuf[idx] = color;
            }
        }
        return true;
    } else {
        return false;
    }
}

void rect_init(mgc_rect_t *rect, mgc_id_t id) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->id = id;
    rect->enabled = MGC_DEFAULT_ENABLED;
    rect->x = 0;
    rect->y = 0;
    rect->r_cell_x_ofs = 1;
    rect->r_cell_y_ofs = 1;
    rect->width = MGC_CELL_LEN;
    rect->height = MGC_CELL_LEN;
    rect->border_width = 0;
    rect->inner_color = MGC_COLOR_SWAP(MGC_COLOR_BLACK);
    rect->border_color = MGC_COLOR_SWAP(MGC_COLOR_WHITE);
}

void rect_set_enabled(mgc_rect_t *rect, bool enabled) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->enabled = enabled;
}

void rect_set_position(mgc_rect_t *rect, int16_t x, int16_t y) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->x = x;
    rect->y = y;
}

void rect_set_width(mgc_rect_t *rect, uint16_t width) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->width = width;
}

void rect_set_height(mgc_rect_t *rect, uint16_t height) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->height = height;
}

void rect_set_border_width(mgc_rect_t *rect, uint16_t border_width) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->border_width = border_width;
}

void rect_set_inner_color(mgc_rect_t *rect, mgc_color_t inner_color) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->inner_color = MGC_COLOR_SWAP(inner_color);
}

void rect_set_border_color(mgc_rect_t *rect, mgc_color_t border_color) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->border_color = MGC_COLOR_SWAP(border_color);
}

void rect_set_r_cell_offset(mgc_rect_t *rect, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->r_cell_x_ofs = r_cell_x_ofs;
    rect->r_cell_y_ofs = r_cell_y_ofs;
}

bool rect_apply_cell_blending(const mgc_rect_t *rect, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {
    draw_box_t box_target, box_cell;
    bool is_blending = false;
    if ( ( rect == NULL ) ||
         ( pixelbuffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( rect->enabled == false ) {
        MGC_INFO("Handler is disabled");
        return false;
    }

    box_cell.left = cell_x;
    box_cell.top  = cell_y;
    if ( rect->r_cell_x_ofs != 0 ) {
        box_cell.left += pixelbuffer->cell_x_ofs / rect->r_cell_x_ofs;
    }
    if ( rect->r_cell_y_ofs != 0 ) {
        box_cell.top += pixelbuffer->cell_y_ofs / rect->r_cell_y_ofs;
    }
    box_cell.right  = box_cell.left + MGC_CELL_LEN - 1;
    box_cell.bottom = box_cell.top  + MGC_CELL_LEN - 1;

    if ( rect->border_width > 0 ) {

        box_target.left   = rect->x - rect->border_width;
        box_target.right  = rect->x + rect->border_width + rect->width - 1;
        box_target.top    = rect->y - rect->border_width;
        box_target.bottom = rect->y + rect->border_width + rect->height - 1;
        if ( apply_cell_blending(pixelbuffer->pixelbuf, &box_target, &box_cell, rect->border_color) == true ) {
            is_blending = true;
        }
    }

    box_target.left   = rect->x;
    box_target.right  = rect->x + rect->width - 1;
    box_target.top    = rect->y;
    box_target.bottom = rect->y + rect->height - 1;
    if ( apply_cell_blending(pixelbuffer->pixelbuf, &box_target, &box_cell, rect->inner_color) == true ) {
        is_blending = true;
    }

    return is_blending;
}

