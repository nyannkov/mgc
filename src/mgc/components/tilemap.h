/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_TILEMAP_H
#define MGC_TILEMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/render/pixelbuffer.h"
#include "map.h"
#include "tileset.h"

typedef struct mgc_tilemap {
    mgc_id_t id;
    int16_t x;
    int16_t y;
    uint8_t r_cell_x_ofs;
    uint8_t r_cell_y_ofs;
    bool enabled;
    const mgc_map_t *map;
    const mgc_tileset_t *tileset;
} mgc_tilemap_t;

void tilemap_init(mgc_tilemap_t *tilemap, mgc_id_t id, const struct mgc_map *map, const mgc_tileset_t *tileset);
void tilemap_set_enabled(mgc_tilemap_t *tilemap, bool enabled);
void tilemap_set_position(mgc_tilemap_t *tilemap, int16_t x, int16_t y);
void tilemap_set_r_cell_offset(mgc_tilemap_t *tilemap, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
bool tilemap_apply_cell_blending(const mgc_tilemap_t *tilemap, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_TILEMAP_H*/
