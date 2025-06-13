/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "hitbox.h"

void hitbox_init(mgc_hitbox_t *hitbox, mgc_id_t id, int8_t x0_ofs, int8_t y0_ofs, uint16_t width, uint16_t height) {
    if ( hitbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    hitbox->id = id;
    hitbox->x0_ofs = x0_ofs;
    hitbox->y0_ofs = y0_ofs;
    hitbox->width = width;
    hitbox->height = height;
    hitbox->enabled = true;
}

void hitbox_set_enabled(mgc_hitbox_t *hitbox, bool enabled) {
    if ( hitbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    hitbox->enabled = enabled;
}

bool hitbox_is_enabled(const mgc_hitbox_t *hitbox) {
    if ( hitbox == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    return hitbox->enabled;
}

const mgc_hitbox_t *hitbox_get_by_id(const mgc_hitbox_t *hitbox_array, size_t hitbox_count, mgc_id_t id) {
    if ( hitbox_array == NULL ) {
        MGC_WARN("Invalid handler");
        return NULL;
    }
    for ( size_t i = 0; i < hitbox_count; i++ ) {
        if ( hitbox_array[i].id == id ) {
            return &hitbox_array[i];
        }
    }

    return NULL;
}

