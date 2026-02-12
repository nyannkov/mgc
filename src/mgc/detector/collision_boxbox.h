/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_BOXBOX_H
#define MGC_COLLISION_BOXBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "collision_common.h"
#include "mgc/components/hitbox.h"

typedef struct mgc_collision_boxbox {
    mgc_world_t box_x;
    mgc_world_t box_y;
    float self_vx;
    float self_vy;
    const mgc_hitbox_t *box;
    mgc_aabb_t aa;
    mgc_contact_t flags;
    mgc_pushback_t pushback;
    float damping;
    mgc_world_t max_overlap_x;
    mgc_world_t max_overlap_y;
} mgc_collision_boxbox_t;

void collision_boxbox_init(
    mgc_collision_boxbox_t* boxbox,
    const mgc_hitbox_t *hitbox,
    float pushback_damping
);

void collision_boxbox_begin(
    mgc_collision_boxbox_t *boxbox,
    mgc_world_t box_x,
    mgc_world_t box_y,
    float self_vx,
    float self_vy
);

bool collision_boxbox_test_hit(
    mgc_collision_boxbox_t *boxbox,
    const mgc_hitbox_t *opp_box, 
    mgc_world_t opp_x,
    mgc_world_t opp_y
);

void collision_boxbox_accumulate_pushback(
    mgc_collision_boxbox_t *boxbox,
    const mgc_hitbox_t *opp_box,
    mgc_world_t opp_x,
    mgc_world_t opp_y,
    enum mgc_corner_push_dir push_dir
);

void collision_boxbox_finalize(
    mgc_collision_boxbox_t *boxbox
);

void collision_boxbox_get_pushback(
    const mgc_collision_boxbox_t *boxbox,
    mgc_pushback_t *out
);

mgc_contact_t collision_boxbox_get_contact_flags(
    const mgc_collision_boxbox_t *boxbox
);

mgc_world_t collision_boxbox_get_max_overlap_x(
    const mgc_collision_boxbox_t *boxbox
);

mgc_world_t collision_boxbox_get_max_overlap_y(
    const mgc_collision_boxbox_t *boxbox
);

bool collision_boxbox_is_fully_blocked(
    const mgc_collision_boxbox_t *boxbox
);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_COLLISION_BOXBOX_H*/
