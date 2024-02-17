/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "hitbox.h"

void hitbox_init(mgc_hitbox_t *hitbox, mgc_id_t id, int8_t x0_ofs, int8_t y0_ofs, uint8_t width, uint8_t height) {
    if ( hitbox == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    hitbox->id = id;
    hitbox->x0_ofs = x0_ofs;
    hitbox->y0_ofs = y0_ofs;
    hitbox->width = width;
    hitbox->height = height;
}

