/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_BOXMAP_H
#define MGC_COLLISION_BOXMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "collision_common.h"
#include "mgc/components/hitbox.h"
#include "mgc/components/map.h"

typedef struct mgc_collision_boxmap {
    mgc_world_t box_x;
    mgc_world_t box_y;
    const mgc_hitbox_t *box;
    mgc_world_t map_x;
    mgc_world_t map_y;
    const mgc_map_t *map;
    mgc_contact_t flags;
} mgc_collision_boxmap_t;

typedef struct mgc_map_range {
    uint16_t col_min;
    uint16_t col_max;
    uint16_t row_min;
    uint16_t row_max;
} mgc_map_range_t;

void collision_boxmap_init(
    mgc_collision_boxmap_t* boxmap,
    const mgc_hitbox_t *hitbox, 
    mgc_world_t box_x,
    mgc_world_t box_y,
    const mgc_map_t *map,
    mgc_world_t map_x,
    mgc_world_t map_y 
);

bool collision_boxmap_calc_map_range(
    const mgc_collision_boxmap_t *boxmap,
    mgc_map_range_t *out
);

void collision_boxmap_begin(
    mgc_collision_boxmap_t *boxmap
);

bool collision_boxmap_test_hit_cell(
    mgc_collision_boxmap_t *boxmap,
    uint16_t row,
    uint16_t col,
    const mgc_map_range_t *map_range,
    uint8_t *hit_map_cell_value
);

bool collision_boxmap_test_hit(
    mgc_collision_boxmap_t *boxmap,
    const mgc_map_range_t *map_range
);

void collision_boxmap_calc_pushback(
    mgc_collision_boxmap_t *boxmap,
    enum mgc_corner_push_dir push_dir,
    mgc_pushback_t *out
);

mgc_contact_t collision_boxmap_get_contact_flags(
    const mgc_collision_boxmap_t *boxmap
);

bool collision_boxmap_is_fully_blocked(
    const mgc_collision_boxmap_t *boxmap
);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_COLLISION_BOXMAP_H*/

