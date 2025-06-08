/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "maphit.h"

static inline void update_hit_info(mgc_maphit_t *maphit) {
    if ( maphit->map_i == maphit->ql ) {
        if ( maphit->map_j == maphit->qt ) {
            if ( !maphit->is_hit_tl ) {
                maphit->is_hit_tl = true;
                maphit->hit_count++;
            }
        } else if ( maphit->map_j == maphit->qb ) {
            if ( !maphit->is_hit_bl ) {
                maphit->is_hit_bl = true;
                maphit->hit_count++;
            }
        } else {
            if ( !maphit->is_hit_l ) {
                maphit->is_hit_l = true;
                maphit->hit_count++;
            }
        }
    } else if ( maphit->map_i == maphit->qr ) {
        if ( maphit->map_j == maphit->qt ) {
            if ( !maphit->is_hit_tr ) {
                maphit->is_hit_tr = true;
                maphit->hit_count++;
            }
        } else if ( maphit->map_j == maphit->qb ) {
            if ( !maphit->is_hit_br ) {
                maphit->is_hit_br = true;
                maphit->hit_count++;
            }
        } else {
            if ( !maphit->is_hit_r ) {
                maphit->is_hit_r = true;
                maphit->hit_count++;
            }
        }
    } else {
        if ( maphit->map_j == maphit->qt ) {
            if ( !maphit->is_hit_t ) {
                maphit->is_hit_t = true;
                maphit->hit_count++;
            }
        } else if ( maphit->map_j == maphit->qb ) {
            if ( !maphit->is_hit_b ) {
                maphit->is_hit_b = true;
                maphit->hit_count++;
            }
        } else {
        }
    }
}

static inline bool is_corner_hit(const mgc_maphit_t *maphit) {
    if ( maphit->hit_count == 1 ) {
        return ( 
            maphit->is_hit_tl ||
            maphit->is_hit_tr || 
            maphit->is_hit_bl || 
            maphit->is_hit_br 
        );
    } else if ( maphit->hit_count == 2 ) {
        return ( 
            ( maphit->is_hit_tl && (maphit->is_hit_t || maphit->is_hit_l) ) ||
            ( maphit->is_hit_tr && (maphit->is_hit_t || maphit->is_hit_r) ) ||
            ( maphit->is_hit_bl && (maphit->is_hit_b || maphit->is_hit_l) ) || 
            ( maphit->is_hit_br && (maphit->is_hit_b || maphit->is_hit_r) ) 
        );
    } else if ( maphit->hit_count == 3 ) {
        return ( 
            ( maphit->is_hit_tl && maphit->is_hit_t && maphit->is_hit_l ) ||
            ( maphit->is_hit_tr && maphit->is_hit_t && maphit->is_hit_r ) ||
            ( maphit->is_hit_bl && maphit->is_hit_b && maphit->is_hit_l ) || 
            ( maphit->is_hit_br && maphit->is_hit_b && maphit->is_hit_r ) 
        );
    } else {
        return false;
    }
}

void maphit_init(mgc_maphit_t *maphit) {
    if ( maphit == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    maphit->hit_x = 0;
    maphit->hit_y = 0;
    maphit->hit_tile_id = 0;
    maphit->hit_count = 0;
    maphit->is_hit_r = false;
    maphit->is_hit_l = false;
    maphit->is_hit_t = false;
    maphit->is_hit_b = false;
    maphit->is_hit_tr = false;
    maphit->is_hit_tl = false;
    maphit->is_hit_br = false;
    maphit->is_hit_bl = false;
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
    if ( tilemap->hit_enabled == false ) {
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
    if ( hitbox->enabled == false ) {
        MGC_INFO("target_hitbox_id is disabled");
        maphit->state = MGC_MAPHIT_STATE_END;
        return;
    }

    maphit->target = target;
    maphit->target_hitbox = hitbox;
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

    maphit->dx = MGC_MOD_CELL_LEN(target->x + hitbox->x0_ofs + - tilemap->x);
    maphit->dy = MGC_MOD_CELL_LEN(target->y + hitbox->y0_ofs + - tilemap->y);

    maphit->hit_count = 0;
    maphit->hit_tile_id = 0;
    maphit->is_hit_r = false;
    maphit->is_hit_l = false;
    maphit->is_hit_t = false;
    maphit->is_hit_b = false;
    maphit->is_hit_tr = false;
    maphit->is_hit_tl = false;
    maphit->is_hit_br = false;
    maphit->is_hit_bl = false;

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
                maphit->hit_tile_id = tile_id;
                maphit->state = MGC_MAPHIT_STATE_IN_PROGRESS;
                update_hit_info(maphit);
                maphit->map_i++;
                return true;
            }
        }
    }
    maphit->state = MGC_MAPHIT_STATE_END;
    return false;
}

void maphit_calc_wall_pushback(
    const mgc_maphit_t *maphit,
    int16_t *pushback_x,
    int16_t *pushback_y,
    enum mgc_corner_escape_type escape_type
) {
    int8_t n_x, n_y;
    int16_t p_x, p_y;

    if ( ( maphit == NULL ) ||
         ( maphit->state != MGC_MAPHIT_STATE_END )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    n_x = (maphit->is_hit_l ?    1 : 0) + (maphit->is_hit_tl ?    1 : 0) + (maphit->is_hit_bl ?    1 : 0)
        + (maphit->is_hit_r ? (-1) : 0) + (maphit->is_hit_tr ? (-1) : 0) + (maphit->is_hit_br ? (-1) : 0);
    n_y = (maphit->is_hit_t ?    1 : 0) + (maphit->is_hit_tl ?    1 : 0) + (maphit->is_hit_tr ?    1 : 0)
        + (maphit->is_hit_b ? (-1) : 0) + (maphit->is_hit_bl ? (-1) : 0) + (maphit->is_hit_br ? (-1) : 0);

    if ( n_x > 0 ) {
        p_x = MGC_CELL_LEN - maphit->dx;
    } else if ( n_x < 0 ) {
        p_x = MGC_MOD_CELL_LEN(maphit->dx + maphit->target_hitbox->width)*-1;
    } else {
        p_x = 0;
    }

    if ( n_y > 0 ) {
        p_y = MGC_CELL_LEN - maphit->dy;
    } else if ( n_y < 0 ) {
        p_y = MGC_MOD_CELL_LEN(maphit->dy + maphit->target_hitbox->height)*-1;
    } else {
        p_y = 0;
    }

    if ( is_corner_hit(maphit) ) {
        if ( MGC_ABS(p_x) > MGC_ABS(p_y) ) {
            p_x = 0;
        } else if ( MGC_ABS(p_x) < MGC_ABS(p_y) ) {
            p_y = 0;
        } else {
            switch (escape_type) {
            case MGC_CORNER_ESCAPE_TYPE_X_DIRECTION:
                p_x = 0;
                break;
            case MGC_CORNER_ESCAPE_TYPE_Y_DIRECTION:
                p_y = 0;
                break;
            default:
                break;
            }
        }
    }

    if ( pushback_x != NULL ) {
        *pushback_x = p_x;
    }
    if ( pushback_y != NULL ) {
        *pushback_y = p_y;
    }
}

