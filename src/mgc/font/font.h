/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_FONT_H
#define MGC_FONT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

#define MGC_FONT_MAX_FONT_SIZE     (16)

typedef struct mgc_glyph {
    uint32_t encoding;
//    int8_t swx0;
//    int8_t swy0;
    int8_t dwx0;
    int8_t dwy0;
    int8_t bb_w;
    int8_t bb_h;
    int8_t bb_xoff0x;
    int8_t bb_yoff0y;
    const uint8_t *bitmap;
} mgc_glyph_t;

typedef struct mgc_font {
    const char *name;
    int8_t fbb_x;
    int8_t fbb_y;
    int8_t x_off;
    int8_t y_off;
    int8_t pixel_size;
    const char *fontname_registry;
    const char *foundry;
    const char *family_name;
    const char *weight_name;
    const char *slant;
    const char *charset_registry;
    const char *charset_encoding;
    const char *copyright;
    const mgc_glyph_t *glyph_array;
    size_t glyph_count;
} mgc_font_t;

const mgc_glyph_t *font_get_glyph_info(const mgc_font_t *font, uint32_t char_code);
bool font_load_bitmap(const mgc_font_t *font, const mgc_glyph_t *glyph, uint32_t *bitmap, size_t bitmap_height);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_FONT_H*/
