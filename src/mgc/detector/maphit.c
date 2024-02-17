/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "maphit.h"

void maphit_init(mgc_maphit_t *maphit) {
    if ( maphit == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    maphit->hit_x = 0;
    maphit->hit_y = 0;
    maphit->qr = 0;
    maphit->ql = 0;
    maphit->qt = 0;
    maphit->qb = 0;
    maphit->map_i = 0;
    maphit->map_j = 0;
    maphit->state = MGC_MAPHIT_STATE_INIT;
    maphit->target = NULL;
    maphit->tilemap = NULL;
}

void maphit_setup_detection(mgc_maphit_t *maphit, const mgc_sprite_t *target, mgc_id_t target_hitbox_id, const mgc_tilemap_t *tilemap) {
    bool hitbox_id_found = false;
    const mgc_hitbox_t *hitbox;
    if ( maphit == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( ( target == NULL ) ||
         ( target->hitbox_array == NULL ) ||
         ( target->hitbox_count == 0 ) ||
         ( tilemap == NULL ) ||
         ( tilemap->map == NULL )
    ) {
        MGC_WARN("Invalid handler");
        maphit->state = MGC_MAPHIT_STATE_ERROR;
        return;
    }
    if ( ( target->enabled == false ) ||
         ( tilemap->enabled == false )
    ) {
        MGC_INFO("Handler is disabled");
        maphit->state = MGC_MAPHIT_STATE_END;
        return;
    }

    for ( size_t idx = 0; idx < target->hitbox_count; idx++ ) {
        hitbox = &target->hitbox_array[idx];
        if ( hitbox->id == target_hitbox_id ) {
            hitbox_id_found = true;
            break;
        }
    }
    if ( hitbox_id_found == false ) {
        MGC_WARN("target_hitbox_id is not found");
        maphit->state = MGC_MAPHIT_STATE_ERROR;
        return;
    }
    if ( ( hitbox->width == 0 ) || ( hitbox->height == 0 ) ) {
        MGC_WARN("Invalid hitbox");
        maphit->state = MGC_MAPHIT_STATE_ERROR;
        return;
    }

    maphit->target = target;
    maphit->tilemap = tilemap;
    maphit->ql = MGC_DIV_CELL_LEN(target->x + hitbox->x0_ofs - tilemap->x);
    maphit->qr = MGC_DIV_CELL_LEN(target->x + hitbox->x0_ofs + hitbox->width - 1 - tilemap->x);
    maphit->qt = MGC_DIV_CELL_LEN(target->y + hitbox->y0_ofs - tilemap->y);
    maphit->qb = MGC_DIV_CELL_LEN(target->y + hitbox->y0_ofs + hitbox->height - 1 - tilemap->y);
    if ( maphit->ql < 0 ) maphit->ql = 0;
    if ( maphit->qt < 0 ) maphit->qt = 0;
    if ( tilemap->map->map_width <= maphit->qr ) maphit->qr = tilemap->map->map_width - 1;
    if ( tilemap->map->map_height <= maphit->qb ) maphit->qb = tilemap->map->map_height - 1;

    maphit->map_i = maphit->ql;
    maphit->map_j = maphit->qt;

    maphit->state = MGC_MAPHIT_STATE_READY;
}

bool maphit_detect(mgc_maphit_t *maphit) {
    const mgc_map_t *map;
    if ( maphit == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( ( maphit->state == MGC_MAPHIT_STATE_INIT ) ||
         ( maphit->state == MGC_MAPHIT_STATE_ERROR ) ||
         ( maphit->state == MGC_MAPHIT_STATE_END )
    ) {
        return false;
    }

    map = maphit->tilemap->map;

    for ( ; maphit->map_j <= maphit->qb; maphit->map_j++, maphit->map_i = maphit->ql ) {
        for ( ; maphit->map_i <= maphit->qr; maphit->map_i++ ) {
            uint8_t tile_id = map_decompress_and_get_tile_id(map, maphit->map_i, maphit->map_j);
            if ( (tile_id  & 0x80) != 0 ) {
                maphit->hit_x = maphit->tilemap->x + MGC_CELL2PIXEL(maphit->map_i);
                maphit->hit_y = maphit->tilemap->y + MGC_CELL2PIXEL(maphit->map_j);
                maphit->state = MGC_MAPHIT_STATE_IN_PROGRESS;
                maphit->map_i++;
                return true;
            }
        }
    }
    maphit->state = MGC_MAPHIT_STATE_END;
    return false;
}

