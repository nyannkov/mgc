/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_COMMON_H
#define MGC_COLLISION_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/components/hitbox.h"

typedef uint8_t mgc_contact_t;

#define MGC_CONTACT_L   (1<<0)
#define MGC_CONTACT_R   (1<<1)
#define MGC_CONTACT_T   (1<<2)
#define MGC_CONTACT_B   (1<<3)
#define MGC_CONTACT_LT  (1<<4)
#define MGC_CONTACT_RT  (1<<5)
#define MGC_CONTACT_LB  (1<<6)
#define MGC_CONTACT_RB  (1<<7)

#define MGC_MASK_CONTACT_ALL \
    (MGC_CONTACT_L|MGC_CONTACT_R|MGC_CONTACT_T|MGC_CONTACT_B|   \
     MGC_CONTACT_LT|MGC_CONTACT_RT|MGC_CONTACT_LB|MGC_CONTACT_RB)

#define MGC_MASK_CONTACT_ALL_CORNER \
    (MGC_CONTACT_LT|MGC_CONTACT_RT|MGC_CONTACT_LB|MGC_CONTACT_RB)

#define MGC_IS_HIT_L(flags)      (((flags)&MGC_CONTACT_L) != 0)
#define MGC_IS_HIT_R(flags)      (((flags)&MGC_CONTACT_R) != 0)
#define MGC_IS_HIT_T(flags)      (((flags)&MGC_CONTACT_T) != 0)
#define MGC_IS_HIT_B(flags)      (((flags)&MGC_CONTACT_B) != 0)
#define MGC_IS_HIT_LT(flags)     (((flags)&MGC_CONTACT_LT) != 0)
#define MGC_IS_HIT_RT(flags)     (((flags)&MGC_CONTACT_RT) != 0)
#define MGC_IS_HIT_RB(flags)     (((flags)&MGC_CONTACT_RB) != 0)
#define MGC_IS_HIT_LB(flags)     (((flags)&MGC_CONTACT_LB) != 0)

typedef struct mgc_aabb {
    mgc_world_t l;
    mgc_world_t r;
    mgc_world_t t;
    mgc_world_t b;
} mgc_aabb_t;

typedef struct mgc_pushback {
    mgc_world_t x;
    mgc_world_t y;
} mgc_pushback_t;

enum mgc_corner_push_dir {
    MGC_CORNER_PUSH_DIR_NONE,
    MGC_CORNER_PUSH_DIR_PREFER_X,
    MGC_CORNER_PUSH_DIR_PREFER_Y,
};

bool collision_point_in_box(
    mgc_world_t x, mgc_world_t y,
    mgc_world_t l, mgc_world_t r,
    mgc_world_t t, mgc_world_t b
);

void collision_calc_aabb_from_hitbox(
    mgc_world_t box_x,
    mgc_world_t box_y,
    const mgc_hitbox_t *hitbox,
    mgc_aabb_t *out
);

bool collision_test_hit(
    const mgc_aabb_t* aa,
    const mgc_aabb_t* bb
);

bool collision_calc_signed_overlap(
    const mgc_aabb_t *aa,
    const mgc_aabb_t *bb,
    mgc_world_t *out_x,
    mgc_world_t *out_y
);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_COLLISION_COMMON_H*/

