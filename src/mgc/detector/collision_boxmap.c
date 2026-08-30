/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#include "collision_boxmap.h"

static bool is_corner_hit_map(mgc_contact_t flags) {

    switch ( flags & MGC_MASK_CONTACT_ALL_CORNER ) {
    case MGC_CONTACT_LT: return (flags & (MGC_CONTACT_R|MGC_CONTACT_B)) == 0;
    case MGC_CONTACT_RT: return (flags & (MGC_CONTACT_L|MGC_CONTACT_B)) == 0;
    case MGC_CONTACT_RB: return (flags & (MGC_CONTACT_L|MGC_CONTACT_T)) == 0;
    case MGC_CONTACT_LB: return (flags & (MGC_CONTACT_R|MGC_CONTACT_T)) == 0;
    default: return false;
    }
}

void collision_boxmap_init(
    mgc_collision_boxmap_t* boxmap,
    const mgc_hitbox_t *hitbox, 
    mgc_world_t box_x,
    mgc_world_t box_y,
    const mgc_map_t *map,
    mgc_world_t map_x,
    mgc_world_t map_y 
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");
    MGC_ASSERT(hitbox != NULL, "`hitbox` must not be NULL");
    MGC_ASSERT(map != NULL, "`map` must not be NULL");

    boxmap->box = hitbox;
    boxmap->box_x = box_x;
    boxmap->box_y = box_y;

    boxmap->map = map;
    boxmap->map_x = map_x;
    boxmap->map_y = map_y;

    boxmap->flags = 0;
}

bool collision_boxmap_calc_map_range(
    const mgc_collision_boxmap_t *boxmap,
    mgc_map_range_t *out
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");
    MGC_ASSERT(boxmap->box != NULL, "`box` must not be NULL");
    MGC_ASSERT(boxmap->map != NULL, "`map` must not be NULL");
    MGC_ASSERT(out != NULL, "`out` must not be NULL");

    mgc_aabb_t target, map_area;

    collision_calc_aabb_from_hitbox(
        boxmap->box_x,
        boxmap->box_y,
        boxmap->box,
        &target
    );

    map_area.l = boxmap->map_x;
    map_area.r = map_area.l + MGC_CELL2PIXEL(boxmap->map->map_width) - 1;
    map_area.t = boxmap->map_y;
    map_area.b = map_area.t + MGC_CELL2PIXEL(boxmap->map->map_height) - 1;

    if ( !collision_test_hit(&target, &map_area) ) {
        return false;
    }

    mgc_world_t col_min = MGC_DIV_CELL_LEN(target.l - boxmap->map_x);
    mgc_world_t col_max = MGC_DIV_CELL_LEN(target.r - boxmap->map_x);
    mgc_world_t row_min = MGC_DIV_CELL_LEN(target.t - boxmap->map_y);
    mgc_world_t row_max = MGC_DIV_CELL_LEN(target.b - boxmap->map_y);

    if ( col_min < 0 ) col_min = 0;
    if ( row_min < 0 ) row_min = 0;
    if ( boxmap->map->map_width <= col_max ) col_max = boxmap->map->map_width - 1;
    if ( boxmap->map->map_height <= row_max ) row_max = boxmap->map->map_height - 1;

    out->col_min = col_min;
    out->col_max = col_max;
    out->row_min = row_min;
    out->row_max = row_max;

    return true;
}

void collision_boxmap_begin(
    mgc_collision_boxmap_t *boxmap
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");

    boxmap->flags = 0;
}

bool collision_boxmap_test_hit_cell(
    mgc_collision_boxmap_t *boxmap,
    uint16_t row,
    uint16_t col,
    const mgc_map_range_t *map_range,
    uint8_t *hit_map_cell_value
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");
    MGC_ASSERT(boxmap->map != NULL, "`map` must not be NULL");
    MGC_ASSERT(map_range != NULL, "`map_range` must not be NULL");

    if ( ( map_range->col_min > col ) ||
         ( map_range->col_max < col ) ||
         ( map_range->row_min > row ) ||
         ( map_range->row_max < row )
    ) {
        MGC_WARN("index is out of range");
        return false;
    }

    uint8_t map_cell_value = map_get_map_cell_value(boxmap->map, col, row);

    if ( !MGC_GET_MAP_HIT_FLAG(map_cell_value) ) {
        return false;
    }

    if ( hit_map_cell_value != NULL ) {
        *hit_map_cell_value = map_cell_value;
    }

    mgc_aabb_t target, cell;

    collision_calc_aabb_from_hitbox(
        boxmap->box_x,
        boxmap->box_y,
        boxmap->box,
        &target
    );

    collision_calc_aabb_from_cell(
        boxmap->map_x,
        boxmap->map_y,
        row,
        col,
        &cell
    );

    boxmap->flags |= collision_calc_contact_flags(&target, &cell);

    return true;
}

bool collision_boxmap_test_hit(
    mgc_collision_boxmap_t *boxmap,
    const mgc_map_range_t *map_range
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");
    MGC_ASSERT(boxmap->map != NULL, "`map` must not be NULL");
    MGC_ASSERT(map_range != NULL, "`map_range` must not be NULL");

    bool r = false;

    for ( uint16_t row = map_range->row_min;; ++row ) {
        for ( uint16_t col = map_range->col_min;; ++col ) {

            r = collision_boxmap_test_hit_cell(boxmap, row, col, map_range, NULL) || r;
            if ( col == map_range->col_max ) break;
        }
        if ( row == map_range->row_max ) break;
    }

    return r;
}


void collision_boxmap_calc_pushback(
    mgc_collision_boxmap_t *boxmap,
    enum mgc_corner_push_dir push_dir,
    mgc_pushback_t *out
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");
    MGC_ASSERT(boxmap->box != NULL, "`box` must not be NULL");
    MGC_ASSERT(boxmap->map != NULL, "`map` must not be NULL");
    MGC_ASSERT(out != NULL, "`out` must not be NULL");

    if ( ( boxmap->flags & MGC_MASK_CONTACT_ALL ) == 0 ) {
        out->x = 0;
        out->y = 0;
        return;
    }

    mgc_contact_t flags = boxmap->flags;
    mgc_world_t dx, dy;
    mgc_world_t width, height;
    int8_t n_x, n_y;
    mgc_world_t p_x, p_y;

    dx = MGC_MOD_CELL_LEN(boxmap->box_x + boxmap->box->x0_ofs - boxmap->map_x);
    dy = MGC_MOD_CELL_LEN(boxmap->box_y + boxmap->box->y0_ofs - boxmap->map_y);

    width = boxmap->box->width;
    height = boxmap->box->height;

    n_x = (MGC_IS_HIT_L(flags) ?    1 : 0) + (MGC_IS_HIT_LT(flags) ?    1 : 0) + (MGC_IS_HIT_LB(flags) ?    1 : 0)
        + (MGC_IS_HIT_R(flags) ? (-1) : 0) + (MGC_IS_HIT_RT(flags) ? (-1) : 0) + (MGC_IS_HIT_RB(flags) ? (-1) : 0);
    n_y = (MGC_IS_HIT_T(flags) ?    1 : 0) + (MGC_IS_HIT_LT(flags) ?    1 : 0) + (MGC_IS_HIT_RT(flags) ?    1 : 0)
        + (MGC_IS_HIT_B(flags) ? (-1) : 0) + (MGC_IS_HIT_LB(flags) ? (-1) : 0) + (MGC_IS_HIT_RB(flags) ? (-1) : 0);

    if ( n_x > 0 ) {
        p_x = MGC_CELL_LEN - dx;
        if ( p_x == 0 ) {
            p_x = MGC_CELL_LEN;
        }
    } else if ( n_x < 0 ) {
        p_x = MGC_MOD_CELL_LEN(dx + width)*-1;
        if ( p_x == 0 ) {
            p_x = MGC_CELL_LEN * -1;
        }
    } else {
        p_x = 0;
    }

    if ( n_y > 0 ) {
        p_y = MGC_CELL_LEN - dy;
        if ( p_y == 0 ) {
            p_y = MGC_CELL_LEN;
        }
    } else if ( n_y < 0 ) {
        p_y = MGC_MOD_CELL_LEN(dy + height)*-1;
        if ( p_y == 0 ) {
            p_y = MGC_CELL_LEN * -1;
        }
    } else {
        p_y = 0;
    }

    if ( is_corner_hit_map(boxmap->flags) ) {
        if ( MGC_ABS(p_x) > MGC_ABS(p_y) ) {
            p_x = 0;
        } else if ( MGC_ABS(p_x) < MGC_ABS(p_y) ) {
            p_y = 0;
        } else {
            switch (push_dir) {
            case MGC_CORNER_PUSH_DIR_PREFER_X:
                p_y = 0;
                break;
            case MGC_CORNER_PUSH_DIR_PREFER_Y:
                p_x = 0;
                break;
            default:
                break;
            }
        }
    }

    out->x = p_x;
    out->y = p_y;
}

mgc_contact_t collision_boxmap_get_contact_flags(
    const mgc_collision_boxmap_t *boxmap
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");
    return boxmap->flags;
}

bool collision_boxmap_is_fully_blocked(
    const mgc_collision_boxmap_t *boxmap
) {
    MGC_ASSERT(boxmap != NULL, "`boxmap` must not be NULL");
    return (boxmap->flags & MGC_MASK_CONTACT_ALL) == MGC_MASK_CONTACT_ALL;
}

