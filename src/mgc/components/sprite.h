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
#include "mgc/render/framebuffer.h"
#include "tileset.h"
#include "hitbox.h"

typedef struct mgc_sprite {
    mgc_id_t id;
    int16_t x;
    int16_t y;
    float parallax_factor_x;
    float parallax_factor_y;
    bool visible;
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
void sprite_set_id(mgc_sprite_t *sprite, mgc_id_t id);
void sprite_set_visible(mgc_sprite_t *sprite, bool v);
void sprite_set_position(mgc_sprite_t *sprite, int16_t x, int16_t y);
void sprite_set_tileset(mgc_sprite_t *sprite, const mgc_tileset_t *tileset);
void sprite_set_tile_index(mgc_sprite_t *sprite, size_t tile_idx);
void sprite_set_hitbox_array(mgc_sprite_t *sprite, const mgc_hitbox_t *hitbox_array, size_t hitbox_count);
void sprite_set_parallax_factor(mgc_sprite_t *sprite, float factor_x, float factor_y);
void sprite_set_trim(mgc_sprite_t *sprite, uint16_t left, uint16_t right, uint16_t top, uint16_t bottom);
bool sprite_draw(const mgc_sprite_t *sprite, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool sprite_draw_cell(
        const mgc_sprite_t *sprite,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool sprite_draw_raw(
        const mgc_sprite_t *sprite,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool sprite_draw_cell_raw(
        const mgc_sprite_t *sprite,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

static inline
mgc_id_t sprite_get_id(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    return sprite->id;
}

static inline
bool sprite_get_visible(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    return sprite->visible;
}

static inline
mgc_point_t sprite_get_position(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    mgc_point_t point = {sprite->x, sprite->y};
    return point;
}

static inline
const mgc_tileset_t* sprite_get_tileset(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    return sprite->tileset;
}

static inline
size_t sprite_get_tile_index(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    return sprite->tile_idx;
}

static inline
const mgc_hitbox_t* sprite_get_hitbox_array(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    return sprite->hitbox_array;
}

static inline
size_t sprite_get_hitbox_count(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    return sprite->hitbox_count;
}

static inline
mgc_parallax_factor_t sprite_get_parallax_factor(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    mgc_parallax_factor_t factor = {
        sprite->parallax_factor_x,
        sprite->parallax_factor_y
    };
    return factor;
}

static inline
mgc_trim_region_t sprite_get_trim_region(const mgc_sprite_t *sprite) {
    MGC_ASSERT(sprite != NULL, "Invalid handler");
    mgc_trim_region_t region = {
        sprite->trim_left,
        sprite->trim_right,
        sprite->trim_top,
        sprite->trim_bottom
    };
    return region;
}

//////////////////////////////// Legacy ////////////////////////////////
bool sprite_apply_cell_blending(const mgc_sprite_t *sprite, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void sprite_set_r_cell_offset(mgc_sprite_t *sprite, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
#define sprite_set_enabled    sprite_set_visible
#define sprite_set_tile_idx   sprite_set_tile_index

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SPRITE_H*/
