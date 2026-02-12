/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#include "collision_boxbox.h"

static inline void accumulate_pushback_pos(mgc_world_t* dst, mgc_world_t v) {
    if (v > 0 && *dst < v) {
        *dst = v;
    }
}

static inline void accumulate_pushback_neg(mgc_world_t* dst, mgc_world_t v) {
    if (v < 0 && *dst > v) {
        *dst = v;
    }
}

static mgc_contact_t calc_contact_flags_box_box(
    const mgc_aabb_t *aa,
    const mgc_aabb_t *bb
) {
    mgc_contact_t flags = 0;
    
    if ( collision_point_in_box(aa->l, aa->t, bb->l, bb->r, bb->t, bb->b) ) {
        flags |= (MGC_CONTACT_LT|MGC_CONTACT_L|MGC_CONTACT_T);
    }
    if ( collision_point_in_box(aa->r, aa->t, bb->l, bb->r, bb->t, bb->b) ) {
        flags |= (MGC_CONTACT_RT|MGC_CONTACT_R|MGC_CONTACT_T);
    }
    if ( collision_point_in_box(aa->l, aa->b, bb->l, bb->r, bb->t, bb->b) ) {
        flags |= (MGC_CONTACT_LB|MGC_CONTACT_L|MGC_CONTACT_B);
    }
    if ( collision_point_in_box(aa->r, aa->b, bb->l, bb->r, bb->t, bb->b) ) {
        flags |= (MGC_CONTACT_RB|MGC_CONTACT_R|MGC_CONTACT_B);
    }

    if ( (aa->t < bb->t) && (bb->b < aa->b) ) {
        if ( (bb->l <= aa->l) && (aa->l <= bb->r) ) {
            flags |= MGC_CONTACT_L;
        }
        if ( (bb->l <= aa->r) && (aa->r <= bb->r) ) {
            flags |= MGC_CONTACT_R;
        }
    }
    if ( (aa->l < bb->l) && (bb->r < aa->r) ) {
        if ( (bb->t <= aa->t) && (aa->t <= bb->b) ) {
            flags |= MGC_CONTACT_T;
        }
        if ( (bb->t <= aa->b) && (aa->b <= bb->b) ) {
            flags |= MGC_CONTACT_B;
        }
    }

    return flags;
}

void collision_boxbox_init(
    mgc_collision_boxbox_t* boxbox,
    const mgc_hitbox_t *hitbox,
    float pushback_damping
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    MGC_ASSERT(hitbox != NULL, "`hitbox` must not be NULL");

    boxbox->box = hitbox;

    boxbox->flags = 0;
    boxbox->damping = pushback_damping;
}

void collision_boxbox_begin(
    mgc_collision_boxbox_t *boxbox,
    mgc_world_t box_x,
    mgc_world_t box_y,
    float self_vx,
    float self_vy
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");

    boxbox->flags = 0;
    boxbox->pushback.x = 0;
    boxbox->pushback.y = 0;
    boxbox->box_x = box_x;
    boxbox->box_y = box_y;
    boxbox->self_vx = self_vx;
    boxbox->self_vy = self_vy;
    boxbox->max_overlap_x = 0;
    boxbox->max_overlap_y = 0;

    collision_calc_aabb_from_hitbox(
        boxbox->box_x,
        boxbox->box_y,
        boxbox->box,
        &boxbox->aa
    );
}

bool collision_boxbox_test_hit(
    mgc_collision_boxbox_t *boxbox,
    const mgc_hitbox_t *opp_box, 
    mgc_world_t opp_x,
    mgc_world_t opp_y
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    MGC_ASSERT(boxbox->box != NULL, "`box` must not be NULL");
    MGC_ASSERT(opp_box != NULL, "`opp_box` must not be NULL");

    if ( !boxbox->box->enabled || !opp_box->enabled ) {
        return false;
    }

    mgc_aabb_t bb;
    collision_calc_aabb_from_hitbox(opp_x, opp_y, opp_box, &bb);

    bool r = collision_test_hit(&boxbox->aa, &bb);
    if ( r ) {
        boxbox->flags |= calc_contact_flags_box_box(&boxbox->aa, &bb);
    }

    return r;
}

void collision_boxbox_accumulate_pushback(
    mgc_collision_boxbox_t *boxbox,
    const mgc_hitbox_t *opp_box,
    mgc_world_t opp_x,
    mgc_world_t opp_y,
    enum mgc_corner_push_dir push_dir
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    MGC_ASSERT(boxbox->box != NULL, "`box` must not be NULL");
    MGC_ASSERT(opp_box != NULL, "`opp_box` must not be NULL");

    const mgc_hitbox_t *self_box = boxbox->box;

    if ( ( self_box->width > opp_box->width ) || 
         ( self_box->height > opp_box->height )
    ) {
        // Currently, this case is not supported and will be considered in the future.
        MGC_WARN("Unsupported case: self_box is larger than opp_box (not handled yet)");
        return;
    }
    mgc_aabb_t bb;
    collision_calc_aabb_from_hitbox(opp_x, opp_y, opp_box, &bb);

    bool r = collision_test_hit(&boxbox->aa, &bb);
    if ( !r ) {
        return;
    }

    mgc_world_t overlap_x = MGC_MIN(boxbox->aa.r, bb.r) - MGC_MAX(boxbox->aa.l, bb.l) + 1;

    boxbox->max_overlap_x = MGC_MAX(boxbox->max_overlap_x, overlap_x);

    // Intentionally truncate pushback to avoid leaving a persistent 1px offset.
    // In the future, this may be replaced with an epsilon-based threshold.
    mgc_world_t n_x = (mgc_world_t)((float)overlap_x * boxbox->damping);

    if ( bb.l <= boxbox->aa.r && boxbox->aa.r < bb.r) {
        n_x *= -1;
    }

    mgc_world_t overlap_y = MGC_MIN(boxbox->aa.b, bb.b) - MGC_MAX(boxbox->aa.t, bb.t) + 1;
    boxbox->max_overlap_y = MGC_MAX(boxbox->max_overlap_y, overlap_y);

    mgc_world_t n_y = (mgc_world_t)((float)overlap_y * boxbox->damping);

    if ( bb.t <= boxbox->aa.b && boxbox->aa.b < bb.b ) {
        n_y *= -1;
    }

    mgc_pushback_t *pushback = &boxbox->pushback;

    switch ( boxbox->flags & MGC_MASK_CONTACT_ALL_CORNER ) {
    case MGC_CONTACT_LT:/*@fall-through@*/
    case (MGC_CONTACT_LB|MGC_CONTACT_LT|MGC_CONTACT_RT):
        if ( overlap_x < overlap_y ||
           ( overlap_x == overlap_y &&
             push_dir == MGC_CORNER_PUSH_DIR_PREFER_X )
        ) {
            accumulate_pushback_pos(&pushback->x, n_x);
        } else if ( overlap_x > overlap_y ||
                  ( overlap_x == overlap_y &&
                    push_dir == MGC_CORNER_PUSH_DIR_PREFER_Y )
        ) {
            accumulate_pushback_pos(&pushback->y, n_y);
        } else {
            accumulate_pushback_pos(&pushback->x, n_x);
            accumulate_pushback_pos(&pushback->y, n_y);
        }
        break;
    case MGC_CONTACT_RT:/*@fall-through@*/
    case (MGC_CONTACT_LT|MGC_CONTACT_RT|MGC_CONTACT_RB):
        if ( overlap_x < overlap_y ||
           ( overlap_x == overlap_y &&
             push_dir == MGC_CORNER_PUSH_DIR_PREFER_X )
        ) {
            accumulate_pushback_neg(&pushback->x, n_x);
        } else if ( overlap_x > overlap_y ||
                  ( overlap_x == overlap_y &&
                    push_dir == MGC_CORNER_PUSH_DIR_PREFER_Y )
        ) {
            accumulate_pushback_pos(&pushback->y, n_y);
        } else {
            accumulate_pushback_neg(&pushback->x, n_x);
            accumulate_pushback_pos(&pushback->y, n_y);
        }
        break;
    case MGC_CONTACT_RB:/*@fall-through@*/
    case (MGC_CONTACT_RT|MGC_CONTACT_RB|MGC_CONTACT_LB):
        if ( overlap_x < overlap_y ||
           ( overlap_x == overlap_y &&
             push_dir == MGC_CORNER_PUSH_DIR_PREFER_X )
        ) {
            accumulate_pushback_neg(&pushback->x, n_x);
        } else if ( overlap_x > overlap_y ||
                  ( overlap_x == overlap_y &&
                    push_dir == MGC_CORNER_PUSH_DIR_PREFER_Y )
        ) {
            accumulate_pushback_neg(&pushback->y, n_y);
        } else {
            accumulate_pushback_neg(&pushback->x, n_x);
            accumulate_pushback_neg(&pushback->y, n_y);
        }
        break;
    case MGC_CONTACT_LB:/*@fall-through@*/
    case (MGC_CONTACT_RB|MGC_CONTACT_LB|MGC_CONTACT_LT):
        if ( overlap_x < overlap_y ||
           ( overlap_x == overlap_y &&
             push_dir == MGC_CORNER_PUSH_DIR_PREFER_X )
        ) {
            accumulate_pushback_pos(&pushback->x, n_x);
        } else if ( overlap_x > overlap_y ||
                  ( overlap_x == overlap_y &&
                    push_dir == MGC_CORNER_PUSH_DIR_PREFER_Y )
        ) {
            accumulate_pushback_neg(&pushback->y, n_y);
        } else {
            accumulate_pushback_pos(&pushback->x, n_x);
            accumulate_pushback_neg(&pushback->y, n_y);
        }
        break;
    case (MGC_CONTACT_LT|MGC_CONTACT_RT):
            accumulate_pushback_pos(&pushback->y, n_y);
        break;
    case (MGC_CONTACT_RT|MGC_CONTACT_RB):
            accumulate_pushback_neg(&pushback->x, n_x);
        break;
    case (MGC_CONTACT_RB|MGC_CONTACT_LB):
            accumulate_pushback_neg(&pushback->y, n_y);
        break;
    case (MGC_CONTACT_LB|MGC_CONTACT_LT):
            accumulate_pushback_pos(&pushback->x, n_x);
        break;

    case (MGC_CONTACT_LT|MGC_CONTACT_RB):/*@fall-throught@*/
    case (MGC_CONTACT_RT|MGC_CONTACT_LB):
        if ( boxbox->self_vx > 0 ) {
            accumulate_pushback_neg(&pushback->x, n_x);
        } else {
            accumulate_pushback_pos(&pushback->x, n_x);
        }
        if ( boxbox->self_vy > 0 ) {
            accumulate_pushback_neg(&pushback->y, n_y);
        } else {
            accumulate_pushback_pos(&pushback->y, n_y);
        }
        break;
    case MGC_MASK_CONTACT_ALL_CORNER:
        // all hit
        break;
    default:
        // Not hit
        break;
    }
}

void collision_boxbox_finalize(
    mgc_collision_boxbox_t *boxbox
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");

    switch ( boxbox->flags & MGC_MASK_CONTACT_ALL_CORNER ) {
    case (MGC_CONTACT_LT|MGC_CONTACT_RT):/*@fall-through@*/
    case (MGC_CONTACT_RB|MGC_CONTACT_LB):
        boxbox->pushback.x = 0;
        break;
    case (MGC_CONTACT_RT|MGC_CONTACT_RB):/*@fall-through@*/
    case (MGC_CONTACT_LB|MGC_CONTACT_LT):
        boxbox->pushback.y = 0;
        break;
    default:
        break;
    }
}

void collision_boxbox_get_pushback(
    const mgc_collision_boxbox_t *boxbox,
    mgc_pushback_t *out
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    MGC_ASSERT(out != NULL, "`out` must not be NULL");

    out->x = boxbox->pushback.x;
    out->y = boxbox->pushback.y;
}

mgc_contact_t collision_boxbox_get_contact_flags(
    const mgc_collision_boxbox_t *boxbox
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    return boxbox->flags;
}

mgc_world_t collision_boxbox_get_max_overlap_x(
    const mgc_collision_boxbox_t *boxbox
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    return boxbox->max_overlap_x;
}

mgc_world_t collision_boxbox_get_max_overlap_y(
    const mgc_collision_boxbox_t *boxbox
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    return boxbox->max_overlap_y;
}

bool collision_boxbox_is_fully_blocked(
    const mgc_collision_boxbox_t *boxbox
) {
    MGC_ASSERT(boxbox != NULL, "`boxbox` must not be NULL");
    return (boxbox->flags & MGC_MASK_CONTACT_ALL_CORNER) == MGC_MASK_CONTACT_ALL_CORNER;
}

