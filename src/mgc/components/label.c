/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "label.h"

void label_init(mgc_label_t *label, mgc_id_t id, const mgc_font_t *font, bool fontsize2x) {
    if ( ( label == NULL ) ||
         ( font == NULL ) ||
         ( font->fbb_x > MGC_FONT_MAX_FONT_SIZE ) ||
         ( font->fbb_y > MGC_FONT_MAX_FONT_SIZE )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->id = id;
    label->x = 0;
    label->y = 0;
    label->enabled = MGC_DEFAULT_ENABLED;
    label->parallax_factor_x = 0.0F;
    label->parallax_factor_y = 0.0F;
    label->text = "";
    label->font = font;
    label->width = font->fbb_x * 10;
    label->height = font->fbb_y;
    label->fore_color = MGC_COLOR_SWAP(MGC_COLOR_WHITE);
    label->back_color = MGC_COLOR_SWAP(MGC_COLOR_BLACK);
    label->fontsize2x = fontsize2x;
    label->enable_back_color = false;
}

void label_set_enabled(mgc_label_t *label, bool enabled) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->enabled = enabled;
}

void label_set_position(mgc_label_t *label, int16_t x, int16_t y) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->x = x;
    label->y = y;
}

void label_set_size(mgc_label_t *label, uint16_t width, uint16_t height) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->width = width;
    label->height = height;
}

void label_set_parallax_factor(mgc_label_t *label, float factor_x, float factor_y) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->parallax_factor_x = factor_x;
    label->parallax_factor_y = factor_y;
}

void label_set_text(mgc_label_t *label, const char *text) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->text = text;
}

void label_set_fore_color(mgc_label_t *label, mgc_color_t fore_color) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->fore_color = MGC_COLOR_SWAP(fore_color);
}

void label_set_back_color(mgc_label_t *label, mgc_color_t back_color) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->back_color = MGC_COLOR_SWAP(back_color);
}

void label_set_enable_back_color(mgc_label_t *label, bool enable) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    label->enable_back_color = enable;
}

static inline bool draw_buffer(
        const mgc_label_t *label,
        mgc_color_t *draw_buf,
        uint16_t buf_width,
        uint16_t buf_height,
        const mgc_point_t *cam_pos,
        const mgc_point_t *fov_ofs,
        const mgc_draw_options_t *options
) {
    // 0: label, 1:camera
    int16_t l0, l1;
    int16_t r0, r1, r0_w;
    int16_t t0, t1;
    int16_t b0, b1;
    const char *p;
    const char *tmp_p;
    int16_t dx;
    int16_t scale;
    int16_t shift;

    if ( ( label == NULL )       ||
         ( label->font == NULL ) ||
         ( draw_buf == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( label->enabled == false ) {
        MGC_INFO("Handler is disabled");
        return false;
    }

    (void)options;

    scale = label->fontsize2x ? 2 : 1;
    shift = scale-1;

    l0 = label->x;
    t0 = label->y;
    b0 = t0 + label->font->fbb_y*scale - 1;
    if ( b0 > (t0 + label->height) ) {
        b0 = t0 + label->height;
    }
    r0_w = l0 + label->width;


    if ( fov_ofs != NULL ) {
        l1 = fov_ofs->x;
        t1 = fov_ofs->y;
    } else {
        l1 = 0;
        t1 = 0;
    }
    if ( cam_pos != NULL ) {
        l1 += MGC_PARALLAX_SHIFT(cam_pos->x, label->parallax_factor_x);
        t1 += MGC_PARALLAX_SHIFT(cam_pos->y, label->parallax_factor_y);
    }
    r1 = l1 + buf_width - 1;
    b1 = t1 + buf_height - 1;

    if ( (r0_w < l1) || (r1 < l0) || (b0 < t1) || (b1 < t0) ) {
        /* Blending not required */
        return false;
    }
    p = label->text;
    while ((*p)&&(l0<=r1)) {
        uint32_t unicode;
        unicode = encoding_utf8_to_unicode(p, &p);
        if ( unicode == 0x0D ) {
            unicode = encoding_utf8_to_unicode(p, &tmp_p);
            if ( unicode == 0x0A ) {
                p = tmp_p;
            }
            break;
        } else if ( (unicode == 0x00) || (unicode == 0x0A) ) { 
            break;
        } else {
            const mgc_glyph_t *glyph;
            glyph = font_get_glyph_info(label->font, unicode);
            dx = glyph->dwx0*scale;
            r0 = l0 + dx - 1;
            if ( r0 >= r0_w ) r0 = r0_w;
            if (l1<=r0) {
                int16_t x_s, y_s, x_e, y_e;
                uint32_t bitmap[MGC_FONT_MAX_FONT_SIZE] = {0};
                font_load_bitmap(label->font, glyph, bitmap, MGC_FONT_MAX_FONT_SIZE);
                x_s = (( l1 < l0 ) ? l0 : l1) - l0;
                x_e = (( r1 < r0 ) ? r1 : r0) - l0;
                y_s = (( t1 < t0 ) ? t0 : t1) - t0;
                y_e = (( b1 < b0 ) ? b1 : b0) - t0;
                for ( int16_t X = x_s; X <= x_e; X++ ) {
                    uint32_t mask_x;
                    if (glyph->bb_w <= 8 ) {
                        mask_x = 0x00010000>>(X>>shift);
                    } else {
                        mask_x = 0x01000000>>(X>>shift);
                    }
                    for ( int16_t Y = y_s; Y <= y_e; Y++ ) {
                        if ( (bitmap[Y>>shift] & mask_x ) != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX(X+l0-l1, Y+t0-t1, buf_width, buf_height);
                            draw_buf[idx] = label->fore_color;
                        } else {
                            if ( label->enable_back_color ) {
                                size_t idx;
                                idx = MGC_GET_PIXELBUF_INDEX(X+l0-l1, Y+t0-t1, buf_width, buf_height);
                                draw_buf[idx] = label->back_color;
                            }
                        }
                    }
                }
            }
            l0 = r0 + 1;
        }
    }
    return true;
}

bool label_draw(const mgc_label_t *label, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options) {

    if ( (fb == NULL) || (fb->buffer == NULL) ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    mgc_point_t fov_ofs = {0, 0};

    return draw_buffer(label, fb->buffer, fb->width, fb->height, cam_pos, &fov_ofs, options);
}

bool label_draw_cell(
        const mgc_label_t *label,
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

    mgc_point_t fov_ofs = {cell_x, cell_y};

    return draw_buffer(label, pb->pixelbuf, MGC_CELL_LEN, MGC_CELL_LEN, cam_pos, &fov_ofs, options);
}

//////////////////////////////// Legacy ////////////////////////////////
bool label_apply_cell_blending(const mgc_label_t *label, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {

    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    mgc_point_t cam_pos = {pixelbuffer->cell_x_ofs, pixelbuffer->cell_y_ofs};

    return label_draw_cell(label, pixelbuffer, cell_x, cell_y, &cam_pos, NULL);
}

void label_set_r_cell_offset(mgc_label_t *label, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( label == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    label->parallax_factor_x = (r_cell_x_ofs != 0) ? (1.0F / r_cell_x_ofs) : 0.0F;
    label->parallax_factor_y = (r_cell_y_ofs != 0) ? (1.0F / r_cell_y_ofs) : 0.0F;
}

