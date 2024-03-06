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
    int16_t hit_x;
    int16_t hit_y;
    uint8_t hit_tile_id;
    uint8_t hit_count;
    bool is_hit_r;
    bool is_hit_l;
    bool is_hit_t;
    bool is_hit_b;
    bool is_hit_tr;
    bool is_hit_tl;
    bool is_hit_br;
    bool is_hit_bl;
    int16_t dx;
    int16_t dy;
    int16_t qr;
    int16_t ql;
    int16_t qt;
    int16_t qb;
    int16_t map_i;
    int16_t map_j;
    enum mgc_maphit_state state;
    const mgc_sprite_t *target;
    const mgc_hitbox_t *target_hitbox;
    const mgc_tilemap_t *tilemap;
} mgc_maphit_t;

#define MAPHIT_GET_HIT_X(maphit) ((maphit).hit_x)
#define MAPHIT_GET_HIT_Y(maphit) ((maphit).hit_y)
#define MAPHIT_GET_HIT_TILE_ID(maphit)  ((maphit).hit_tile_id)

extern void maphit_init(mgc_maphit_t *maphit);
extern void maphit_setup_detection(mgc_maphit_t *maphit, const mgc_sprite_t *target, mgc_id_t target_hitbox_id, const mgc_tilemap_t *tilemap);
extern bool maphit_detect(mgc_maphit_t *maphit);
extern void maphit_calc_wall_pushback(const mgc_maphit_t *maphit, int16_t *pushback_x, int16_t *pushback_y, enum mgc_corner_escape_type escape_type);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_MAPHIT_H*/
