/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "actor_if.hpp"

namespace mgc {

bool check_hit_tilemap(ActorIf *actor, mgc_maphit &maphit, const mgc_tilemap &tilemap) {
    if ( actor == nullptr ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    bool hit_flag = false;
    const mgc_sprite *sprite = actor->get_ptr_sprite();
    for ( size_t idx = 0; idx < sprite->hitbox_count; idx++ ) {
        const mgc_hitbox &hitbox = sprite->hitbox_array[idx];
        maphit_setup_detection(&maphit, sprite, hitbox.id, &tilemap);
        while ( maphit_detect(&maphit) ) {
            actor->on_hit_tilemap(hitbox, MAPHIT_GET_HIT_X(maphit), MAPHIT_GET_HIT_Y(maphit));
            hit_flag = true;
        }
    }
    return hit_flag;
}

bool check_hit_sprite(ActorIf *actor, mgc_sprhit &sprhit, const mgc_sprite &opp_spr) {
    if ( actor == nullptr ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    bool hit_flag = false;
    const mgc_sprite *tar_spr = actor->get_ptr_sprite();
    for ( size_t idx = 0; idx < tar_spr->hitbox_count; idx++ ) {
        const mgc_hitbox &hitbox = tar_spr->hitbox_array[idx];
        sprhit_setup_detection(&sprhit, tar_spr, hitbox.id, &opp_spr);
        while ( sprhit_detect(&sprhit) ) {
            actor->on_hit_sprite(hitbox, opp_spr);
            hit_flag = true;
        }
    }
    return hit_flag;
}

}
