/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_MAPHIT_H
#define MGC_MAPHIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/components/sprite.h"
#include "mgc/components/tilemap.h"

enum mgc_maphit_state {
    MGC_MAPHIT_STATE_INIT,
    MGC_MAPHIT_STATE_READY,
    MGC_MAPHIT_STATE_IN_PROGRESS,
    MGC_MAPHIT_STATE_END,
    MGC_MAPHIT_STATE_ERROR
};

enum mgc_corner_escape_type {
    MGC_CORNER_ESCAPE_TYPE_X_DIRECTION = 0,
    MGC_CORNER_ESCAPE_TYPE_Y_DIRECTION,
    MGC_CORNER_ESCAPE_TYPE_NO_ESCAPE
};

typedef struct mgc_maphit {
    mgc_world_t hit_x;
    mgc_world_t hit_y;
    uint8_t hit_map_cell_value;
    uint8_t hit_count;
    bool is_hit_r;
    bool is_hit_l;
    bool is_hit_t;
    bool is_hit_b;
    bool is_hit_tr;
    bool is_hit_tl;
    bool is_hit_br;
    bool is_hit_bl;
    int32_t dx;
    int32_t dy;
    int32_t qr;
    int32_t ql;
    int32_t qt;
    int32_t qb;
    int32_t map_i;
    int32_t map_j;
    enum mgc_maphit_state state;
    const mgc_sprite_t *target;
    const mgc_hitbox_t *target_hitbox;
    const mgc_tilemap_t *tilemap;
} mgc_maphit_t;

#define MAPHIT_GET_HIT_X(maphit) ((maphit).hit_x)
#define MAPHIT_GET_HIT_Y(maphit) ((maphit).hit_y)

#define MAPHIT_GET_HIT_MAP_CELL_VALUE(maphit)  ((maphit).hit_map_cell_value)

extern void maphit_init(mgc_maphit_t *maphit);
extern void maphit_setup_detection(mgc_maphit_t *maphit, const mgc_sprite_t *target, mgc_id_t target_hitbox_id, const mgc_tilemap_t *tilemap);
extern bool maphit_detect(mgc_maphit_t *maphit);
extern void maphit_calc_wall_pushback(const mgc_maphit_t *maphit, mgc_world_t *pushback_x, mgc_world_t *pushback_y, enum mgc_corner_escape_type escape_type);

//////////////////////////////// Legacy ////////////////////////////////
#define MAPHIT_GET_HIT_TILE_ID  MAPHIT_GET_HIT_MAP_CELL_VALUE

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_MAPHIT_H*/
