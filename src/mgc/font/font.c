/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "font.h"

#define WIDTH_8     8
#define WIDTH_16    16

const mgc_glyph_t *font_get_glyph_info(const mgc_font_t *font, uint32_t char_code) {
    const mgc_glyph_t *glyph_array;
    size_t low, high, mid;
    uint32_t code;

    if ( font == NULL ) {
        MGC_WARN("Invalid handler");
        return NULL;
    }

    glyph_array = font->glyph_array;

    low = 0;
    high = font->glyph_count - 1;

    while ( low <= high ) {
        mid = (low + high)>>1;
        code = glyph_array[mid].encoding;

        if ( code == char_code ) {
            return &glyph_array[mid];
        } else if ( char_code < code ) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return &glyph_array[0];/* space */
}

bool font_load_bitmap(const mgc_font_t *font, const mgc_glyph_t *glyph, uint32_t *bitmap, size_t bitmap_height) {
    int16_t i;
    int16_t x_ofs, y_ofs;
    int16_t y_len;
    uint32_t shift;

    if ( ( font == NULL ) ||
         ( bitmap == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    x_ofs = glyph->bb_xoff0x - font->x_off;
    shift = 8 - x_ofs;
    y_len = font->fbb_y + font->y_off - glyph->bb_yoff0y;
    if ( y_len > bitmap_height ) y_len = bitmap_height;
    y_ofs = y_len - glyph->bb_h;
    if ( y_ofs < 0 ) y_ofs = 0;

    if ( glyph->bb_w <= WIDTH_8 ) {
        for ( i = y_ofs; i < y_len; i++ ) {
            bitmap[i] = ((uint32_t)glyph->bitmap[i-y_ofs]) << shift;
        }
    } else if ( glyph->bb_w <= WIDTH_16 ) {
        size_t idx;
        for ( i = y_ofs; i < y_len; i++ ) {
            idx = (i-y_ofs)<<1;
            bitmap[i] = (((uint32_t)glyph->bitmap[idx])<<8|(glyph->bitmap[idx+1])) << shift;
        }
    } else {
        MGC_WARN("Unsupported font width");
        return false;
    }
    return true;
}
