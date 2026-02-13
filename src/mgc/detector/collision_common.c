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
    MGC_ASSERT(hitbox->width > 0, "`hitbox->width` must be greater than 0");
    MGC_ASSERT(hitbox->height > 0, "`hitbox->height` must be greater than 0");
    MGC_ASSERT(out != NULL, "`out` must not be NULL");

    out->l = box_x + hitbox->x0_ofs;
    out->r = out->l + hitbox->width - 1;
    out->t = box_y + hitbox->y0_ofs;
    out->b = out->t + hitbox->height - 1;
}

void collision_expand_aabb_margin(
    const mgc_aabb_t* src,
    const mgc_aabb_margin_t* m,
    mgc_aabb_t* out
) {
    MGC_ASSERT(src != NULL, "`src` must not be NULL");
    MGC_ASSERT(m != NULL, "`m` must not be NULL");
    MGC_ASSERT(out != NULL, "`out` must not be NULL");

    MGC_ASSERT(m->l >= 0, "`m->l` must be >= 0");
    MGC_ASSERT(m->r >= 0, "`m->r` must be >= 0");
    MGC_ASSERT(m->t >= 0, "`m->t` must be >= 0");
    MGC_ASSERT(m->b >= 0, "`m->b` must be >= 0");

    out->l = src->l - m->l;
    out->r = src->r + m->r;
    out->t = src->t - m->t;
    out->b = src->b + m->b;
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

bool collision_calc_signed_overlap(
    const mgc_aabb_t *aa,
    const mgc_aabb_t *bb,
    mgc_world_t *out_x,
    mgc_world_t *out_y
) {
    MGC_ASSERT(aa != NULL, "`aa` must not be NULL");
    MGC_ASSERT(bb != NULL, "`bb` must not be NULL");

    /* inclusive AABB: [l, r], [t, b] */
    mgc_world_t overlap_x = MGC_MIN(aa->r, bb->r) - MGC_MAX(aa->l, bb->l) + 1;
    mgc_world_t overlap_y = MGC_MIN(aa->b, bb->b) - MGC_MAX(aa->t, bb->t) + 1;

    /* no intersection */
    if ( overlap_x < 0 || overlap_y < 0 ) {
        MGC_WARN("aa and bb do not overlap.");
        return false;
    }

    mgc_world_t center_a_x = aa->l + (aa->r - aa->l) / 2;
    mgc_world_t center_b_x = bb->l + (bb->r - bb->l) / 2;

    mgc_world_t center_a_y = aa->t + (aa->b - aa->t) / 2;
    mgc_world_t center_b_y = bb->t + (bb->b - bb->t) / 2;

    /* determine sign (direction to separate aa from bb) */
    if ( center_a_x < center_b_x ) {
        overlap_x *= -1;
    }

    if ( center_a_y < center_b_y ) {
        overlap_y *= -1;
    }
    
    if ( out_x != NULL ) {
        *out_x = overlap_x;
    }

    if ( out_y != NULL ) {
        *out_y = overlap_y;
    }

    return true;
}

