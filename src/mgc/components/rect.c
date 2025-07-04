/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "rect.h"

void rect_init(mgc_rect_t *rect, mgc_id_t id) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->id = id;
    rect->visible = MGC_DEFAULT_VISIBLE;
    rect->x = 0;
    rect->y = 0;
    rect->parallax_factor_x = 1.0F;
    rect->parallax_factor_y = 1.0F;
    rect->width = MGC_CELL_LEN;
    rect->height = MGC_CELL_LEN;
    rect->border_width = 0;
    rect->inner_color = MGC_COLOR_BLACK;
    rect->border_color = MGC_COLOR_WHITE;
}

void rect_set_id(mgc_rect_t *rect, mgc_id_t id) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->id = id;
}

void rect_set_visible(mgc_rect_t *rect, bool v) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->visible = v;
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
    rect->inner_color = inner_color;
}

void rect_set_border_color(mgc_rect_t *rect, mgc_color_t border_color) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->border_color = border_color;
}

void rect_set_parallax_factor(mgc_rect_t *rect, float factor_x, float factor_y) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    rect->parallax_factor_x = factor_x;
    rect->parallax_factor_y = factor_y;
}

static inline bool draw_buffer(
        const mgc_rect_t *rect,
        mgc_color_t *draw_buf,
        uint16_t buf_width,
        uint16_t buf_height,
        const mgc_point_t *cam_pos,
        const mgc_point_t *fov_ofs,
        const mgc_draw_options_t *options
) {
    // 0: rect, 1: camera
    int16_t l0, l1;
    int16_t r0, r1;
    int16_t t0, t1;
    int16_t b0, b1;

    if ( ( rect == NULL ) ||
         ( draw_buf == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( rect->visible == false ) {
        MGC_INFO("Handler is not visible");
        return false;
    }

    (void)options;

    l0 = rect->x - rect->border_width;
    t0 = rect->y - rect->border_width;
    r0 = l0 + rect->border_width + rect->width - 1;
    b0 = t0 + rect->border_width + rect->height - 1;

    if ( fov_ofs != NULL ) {
        l1 = fov_ofs->x;
        t1 = fov_ofs->y;
    } else {
        l1 = 0;
        t1 = 0;
    }
    if ( cam_pos != NULL ) {
        l1 += MGC_PARALLAX_SHIFT(cam_pos->x, rect->parallax_factor_x);
        t1 += MGC_PARALLAX_SHIFT(cam_pos->y, rect->parallax_factor_y);
    }
    r1 = l1 + buf_width - 1;
    b1 = t1 + buf_height - 1;

    if ( (l0<=r1) && (l1<=r0) && (t0<=b1) && (t1<=b0) ) {
        int16_t x, y;
        int16_t x_s, y_s, x_e, y_e;
        mgc_color_t color;

        if ( rect->border_width > 0 ) {

            color = MGC_COLOR_SWAP(rect->border_color);
            x_s = (( l1 < l0 ) ? l0 : l1) - l0;
            x_e = (( r1 < r0 ) ? r1 : r0) - l0;
            y_s = (( t1 < t0 ) ? t0 : t1) - t0;
            y_e = (( b1 < b0 ) ? b1 : b0) - t0;

            for ( x = x_s; x <= x_e; x++ ) {
                for ( y = y_s; y <= y_e; y++ ) {
                    size_t idx = MGC_GET_PIXELBUF_INDEX(x+l0-l1, y+t0-t1, buf_width, buf_height);
                    draw_buf[idx] = color;
                }
            }
            l0 += rect->border_width;
            t0 += rect->border_width;
            r0 -= rect->border_width;
            b0 -= rect->border_width;
        }

        color = MGC_COLOR_SWAP(rect->inner_color);
        x_s = (( l1 < l0 ) ? l0 : l1) - l0;
        x_e = (( r1 < r0 ) ? r1 : r0) - l0;
        y_s = (( t1 < t0 ) ? t0 : t1) - t0;
        y_e = (( b1 < b0 ) ? b1 : b0) - t0;

        for ( x = x_s; x <= x_e; x++ ) {
            for ( y = y_s; y <= y_e; y++ ) {
                size_t idx = MGC_GET_PIXELBUF_INDEX(x+l0-l1, y+t0-t1, buf_width, buf_height);
                draw_buf[idx] = color;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool rect_draw(const mgc_rect_t *rect, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options) {

    if ( (fb == NULL) || (fb->buffer == NULL) ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    return rect_draw_raw(rect, fb->buffer, fb->width, fb->height, cam_pos, options);
}

bool rect_draw_cell(
        const mgc_rect_t *rect,
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

    return rect_draw_cell_raw(rect, pb->pixelbuf, cell_x, cell_y, cam_pos, options);
}

bool rect_draw_raw(
        const mgc_rect_t *rect,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    mgc_point_t fov_ofs = {0, 0};

    return draw_buffer(rect, buffer, width, height, cam_pos, &fov_ofs, options);
}

bool rect_draw_cell_raw(
        const mgc_rect_t *rect,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    mgc_point_t fov_ofs = {cell_x, cell_y};

    return draw_buffer(rect, cell_buffer, MGC_CELL_LEN, MGC_CELL_LEN, cam_pos, &fov_ofs, options);
}

//////////////////////////////// Legacy ////////////////////////////////
bool rect_apply_cell_blending(const mgc_rect_t *rect, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {

    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    mgc_point_t cam_pos = {pixelbuffer->cell_x_ofs, pixelbuffer->cell_y_ofs};

    return rect_draw_cell(rect, pixelbuffer, cell_x, cell_y, &cam_pos, NULL);
}

void rect_set_r_cell_offset(mgc_rect_t *rect, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( rect == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    rect->parallax_factor_x = (r_cell_x_ofs != 0) ? (1.0F / r_cell_x_ofs) : 0.0F;
    rect->parallax_factor_y = (r_cell_y_ofs != 0) ? (1.0F / r_cell_y_ofs) : 0.0F;
}

