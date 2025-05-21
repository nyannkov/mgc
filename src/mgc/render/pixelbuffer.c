/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "pixelbuffer.h"

static inline void pixelbuf_set(mgc_pixelbuffer_t *pixelbuffer, mgc_color_t color_swapped) {
    int n = 0;
    while ( n < PIXELBUF_LEN ) {
        pixelbuffer->pixelbuf[n++] = color_swapped;
    }
}

void pixelbuffer_init(mgc_pixelbuffer_t *pixelbuffer) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    pixelbuffer->back_color = MGC_COLOR_SWAP(0);
    pixelbuffer->refresh_mode = true;
    pixelbuffer->cell_x_ofs = 0;
    pixelbuffer->cell_y_ofs = 0;
    pixelbuf_set(pixelbuffer, pixelbuffer->back_color);
}

void pixelbuffer_reset_buffer(mgc_pixelbuffer_t *pixelbuffer) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    pixelbuf_set(pixelbuffer, pixelbuffer->back_color);
}

void pixelbuffer_fill_with_color(mgc_pixelbuffer_t *pixelbuffer, mgc_color_t color) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    pixelbuf_set(pixelbuffer, MGC_COLOR_SWAP(color));
}

void pixelbuffer_fill_partial_with_color(mgc_pixelbuffer_t *pixelbuffer, mgc_color_t color, int16_t fill_dx, int16_t fill_dy) {
    int16_t x0, wy0, x1, wy1;
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( (fill_dx == 0) || (fill_dy == 0) ) {
        MGC_INFO("fill_dx or fill_dy is 0.");
        return;
    }

    if ( fill_dx > 0 ) {
        x0 = 0;
        x1 = MGC_MOD_CELL_LEN(fill_dx-1);
    } else {
        x0 = MGC_MOD_CELL_LEN(fill_dx);
        x1 = MGC_CELL_LEN-1;
    }
    if ( fill_dy > 0 ) {
        wy0 = 0;
        wy1 = MGC_MOD_CELL_LEN(fill_dy-1)<<MGC_CELL_LEN_LOG2;
    } else {
        wy0 = MGC_MOD_CELL_LEN(fill_dy)<<MGC_CELL_LEN_LOG2;
        wy1 = (MGC_CELL_LEN-1)<<MGC_CELL_LEN_LOG2;
    }

    for ( int16_t wy = wy0; wy <= wy1; wy+=MGC_CELL_LEN ) {
        for ( int16_t x = x0; x <= x1; x++ ) {
            pixelbuffer->pixelbuf[x+wy] = MGC_COLOR_SWAP(color);
        }
    }
}

void pixelbuffer_set_back_color(mgc_pixelbuffer_t *pixelbuffer, mgc_color_t back_color) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    pixelbuffer->back_color = MGC_COLOR_SWAP(back_color);
}

void pixelbuffer_set_refresh_mode(mgc_pixelbuffer_t *pixelbuffer, bool refresh_mode) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    pixelbuffer->refresh_mode = refresh_mode;
}

//////////////////////////////// Legacy ////////////////////////////////
void pixelbuffer_set_cell_offset(mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x_ofs, int16_t cell_y_ofs) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    pixelbuffer->cell_x_ofs = cell_x_ofs;
    pixelbuffer->cell_y_ofs = cell_y_ofs;
}

void pixelbuffer_add_cell_offset(mgc_pixelbuffer_t *pixelbuffer, int16_t dx, int16_t dy) {
    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    pixelbuffer->cell_x_ofs += dx;
    pixelbuffer->cell_y_ofs += dy;
}

void pixelbuffer_draw_cell(mgc_pixelbuffer_t *pixelbuffer, const mgc_display_if_t *driver, int16_t cell_x, int16_t cell_y) {
    if ( ( pixelbuffer == NULL ) ||
         ( driver == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    driver->draw_rect((uint8_t *)pixelbuffer->pixelbuf, sizeof(pixelbuffer->pixelbuf), cell_x, cell_y, cell_x+MGC_CELL_LEN-1, cell_y+MGC_CELL_LEN-1);
    if ( pixelbuffer->refresh_mode ) {
        pixelbuf_set(pixelbuffer, pixelbuffer->back_color);
    }
}

mgc_color_t *pixelbuffer_get_buffer(const mgc_pixelbuffer_t *pb) {
    if ( pb == NULL ) {
        MGC_WARN("Invalid handler");
        return NULL;
    }
    return pb->pixelbuf;
}

uint16_t pixelbuffer_get_width(const mgc_pixelbuffer_t *pb) {
    (void)pb;
    return MGC_CELL_LEN;
}

uint16_t pixelbuffer_get_height(const mgc_pixelbuffer_t *pb) {
    (void)pb;
    return MGC_CELL_LEN;
}

