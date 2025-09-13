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
#include "mgc/render/framebuffer.h"
#include "map.h"
#include "tileset.h"

typedef struct mgc_tilemap_callbacks {
    void *context;
    uint8_t (*on_get_map_cell_value)(uint8_t map_cell_value, uint16_t row, uint16_t col, void *context);
} mgc_tilemap_callbacks_t;

typedef struct mgc_tilemap {
    mgc_id_t id;
    int16_t x;
    int16_t y;
    float parallax_factor_x;
    float parallax_factor_y;
    bool hit_enabled;
    bool visible;
    const mgc_map_t *map;
    const mgc_tileset_t *tileset;
    mgc_tilemap_callbacks_t callbacks;
} mgc_tilemap_t;


void tilemap_init(mgc_tilemap_t *tilemap, mgc_id_t id, const mgc_map_t *map, const mgc_tileset_t *tileset);
void tilemap_set_id(mgc_tilemap_t *tilemap, mgc_id_t id);
void tilemap_set_map(mgc_tilemap_t *tilemap,const mgc_map_t *map);
void tilemap_set_tileset(mgc_tilemap_t *tilemap,const mgc_tileset_t *tileset);

void tilemap_set_hit_enabled(mgc_tilemap_t *tilemap, bool enabled);
void tilemap_set_visible(mgc_tilemap_t *tilemap, bool v);
void tilemap_set_position(mgc_tilemap_t *tilemap, int16_t x, int16_t y);
void tilemap_set_parallax_factor(mgc_tilemap_t *tilemap, float factor_x, float factor_y);
void tilemap_set_callbacks(mgc_tilemap_t *tilemap, const mgc_tilemap_callbacks_t *callbacks);
bool tilemap_draw(const mgc_tilemap_t *tilemap, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options);
bool tilemap_draw_cell(
        const mgc_tilemap_t *tilemap,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool tilemap_draw_raw(
        const mgc_tilemap_t *tilemap,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);
bool tilemap_draw_cell_raw(
        const mgc_tilemap_t *tilemap,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
);

static inline
mgc_id_t tilemap_get_id(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    return tilemap->id;
}

static inline
const mgc_map_t* tilemap_get_map(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    return tilemap->map;
}

static inline
const mgc_tileset_t* tilemap_get_tileset(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    return tilemap->tileset;
}

static inline
bool tilemap_get_visible(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    return tilemap->visible;
}

static inline
mgc_point_t tilemap_get_position(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    mgc_point_t point = {tilemap->x, tilemap->y};
    return point;
}

static inline
mgc_parallax_factor_t tilemap_get_parallax_factor(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    mgc_parallax_factor_t factor = {
        tilemap->parallax_factor_x,
        tilemap->parallax_factor_y
    };
    return factor;
}

static inline
bool tilemap_get_hit_enabled(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    return tilemap->hit_enabled;
}

static inline
const mgc_tilemap_callbacks_t * 
tilemap_get_tilemap_callbacks(const mgc_tilemap_t *tilemap) {
    MGC_ASSERT(tilemap != NULL, "Invalid handler");
    return &tilemap->callbacks;
}

//////////////////////////////// Legacy ////////////////////////////////
bool tilemap_apply_cell_blending(const mgc_tilemap_t *tilemap, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y);
void tilemap_set_r_cell_offset(mgc_tilemap_t *tilemap, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs);
void tilemap_set_on_get_map_cell_value(mgc_tilemap_t *tilemap, uint8_t (*cb)(uint8_t map_cell_value, uint16_t row, uint16_t col, void *context), void *context);
#define tilemap_set_enabled    tilemap_set_visible
#define tilemap_get_tilemap_on_get_tile_id_cb(tilemap)    tilemap_get_tilemap_callbacks((tilemap))->on_get_map_cell_value

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_TILEMAP_H*/
