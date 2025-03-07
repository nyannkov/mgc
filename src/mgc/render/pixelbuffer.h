/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_PIXELBUFFER_H
#define MGC_PIXELBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "display.h"

#define PIXELBUF_LEN           (MGC_CELL_LEN*MGC_CELL_LEN)

typedef struct mgc_pixelbuffer {
    mgc_color_t pixelbuf[PIXELBUF_LEN];
    mgc_color_t back_color;
    bool refresh_mode;
    int16_t cell_x_ofs;
    int16_t cell_y_ofs;
} mgc_pixelbuffer_t;

void pixelbuffer_init(mgc_pixelbuffer_t *pixelbuffer);
void pixelbuffer_reset_buffer(mgc_pixelbuffer_t *pixelbuffer);
void pixelbuffer_fill_with_color(mgc_pixelbuffer_t *pixelbuffer, mgc_color_t color);
void pixelbuffer_fill_partial_with_color(mgc_pixelbuffer_t *pixelbuffer, mgc_color_t color, int16_t fill_dx, int16_t fill_dy);
void pixelbuffer_set_refresh_mode(mgc_pixelbuffer_t *pixelbuffer, bool refresh_mode);
void pixelbuffer_set_back_color(mgc_pixelbuffer_t *pixelbuffer, mgc_color_t back_color);
void pixelbuffer_set_cell_offset(mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x_ofs, int16_t cell_y_ofs);
void pixelbuffer_add_cell_offset(mgc_pixelbuffer_t *pixelbuffer, int16_t dx, int16_t dy);
void pixelbuffer_draw_cell(mgc_pixelbuffer_t *pixelbuffer, const mgc_display_if_t *driver, int16_t cell_x, int16_t cell_y);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_PIXELBUFFER_H*/
