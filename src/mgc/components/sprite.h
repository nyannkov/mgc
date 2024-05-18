/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SPRITE_H
#define MGC_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/render/pixelbuffer.h"
#include "tileset.h"
#include "tilemap.h"
#include "hitbox.h"

typedef struct mgc_sprite {
    mgc_id_t id;
    int16_t x;
    int16_t y;
    uint8_t r_cell_x_ofs;
    uint8_t r_cell_y_ofs;
    bool enabled;
    const struct mgc_tileset *tileset;
    size_t tile_idx;
    const mgc_hitbox_t *hitbox_array;
    size_t hitbox_count;
    uint16_t trim_left;
    uint16_t trim_right;
    uint16_t trim_top;
    uint16_t trim_bottom;
} mgc_sprite_t;

void sprite_init(mgc_sprite_t *sprite, mgc_id_t id);
void sprite_set_enabled(mgc_sprite_t *sprite, bool enabled);
void sprite_set_position(mgc_sprite_t *sprite, int16_t x, int16_t y);
void sprite_set_tileset(mgc_sprite_t *sprite, const mgc_tileset_t *tileset);
void sprite_set_tile_idx(mgc_sprite_t *sprite, size_t tile_idx);
void sprite_set_hitbox_array(mgc_sprite_t *sprite, const mgc_hitbox_t *hitbox_array, size_t hitbox_count);
void sprite_set_r_cell_offset(mgc_sprite_t *sprite, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
void sprite_set_trim(mgc_sprite_t *sprite, uint16_t left, uint16_t right, uint16_t top, uint16_t bottom);
bool sprite_apply_cell_blending(const mgc_sprite_t *sprite, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SPRITE_H*/
