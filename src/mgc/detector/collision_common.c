/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#include "collision_common.h"

bool collision_point_in_box(
    mgc_world_t x, mgc_world_t y,
    mgc_world_t l, mgc_world_t r,
    mgc_world_t t, mgc_world_t b
) {
    return (l <= x && x <= r &&
            t <= y && y <= b);
}

void collision_calc_aabb_from_hitbox(
    mgc_world_t box_x,
    mgc_world_t box_y,
    const mgc_hitbox_t *hitbox,
    mgc_aabb_t *out
) {
    MGC_ASSERT(hitbox != NULL, "`hitbox` must not be NULL");
    MGC_ASSERT(out != NULL, "`out` must not be NULL");

    out->l = box_x + hitbox->x0_ofs;
    out->r = out->l + hitbox->width - 1;
    out->t = box_y + hitbox->y0_ofs;
    out->b = out->t + hitbox->height - 1;
}

bool collision_test_hit(const mgc_aabb_t* aa, const mgc_aabb_t* bb) {
    MGC_ASSERT(aa != NULL, "`aa` must not be NULL");
    MGC_ASSERT(bb != NULL, "`bb` must not be NULL");
    
    return (
        (aa->l <= bb->r) && 
        (bb->l <= aa->r) && 
        (aa->t <= bb->b) && 
        (bb->t <= aa->b)
    );
}

