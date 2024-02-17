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

typedef struct mgc_maphit {
    int16_t hit_x;
    int16_t hit_y;
    int16_t qr;
    int16_t ql;
    int16_t qt;
    int16_t qb;
    int16_t map_i;
    int16_t map_j;
    enum mgc_maphit_state state;
    const mgc_sprite_t *target;
    const mgc_tilemap_t *tilemap;
} mgc_maphit_t;

#define MAPHIT_GET_HIT_X(maphit) ((maphit).hit_x)
#define MAPHIT_GET_HIT_Y(maphit) ((maphit).hit_y)

extern void maphit_init(mgc_maphit_t *maphit);
extern void maphit_setup_detection(mgc_maphit_t *maphit, const mgc_sprite_t *target, mgc_id_t target_hitbox_id, const mgc_tilemap_t *tilemap);
extern bool maphit_detect(mgc_maphit_t *maphit);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_MAPHIT_H*/
