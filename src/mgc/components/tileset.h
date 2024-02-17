/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_TILESET_H
#define MGC_TILESET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef struct mgc_tileset {
    enum mgc_pixel_format pixel_format;
    uint16_t tile_width;
    uint16_t tile_height;
    const mgc_color_t *palette_array;
    size_t palette_count;
    const uint8_t **tile_array;
    size_t tile_count;
} mgc_tileset_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_TILESET_H*/
