/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_ACTOR_IF_HPP
#define MGC_ACTOR_IF_HPP

#include "mgc/mgc.h"

namespace mgc {

struct ActorIf {
    virtual void init(mgc_id_t id, int16_t x, int16_t y) = 0;
    virtual const mgc_sprite *get_ptr_sprite() const = 0;
    virtual void update() {}
    virtual void on_hit_tilemap(const mgc_hitbox &hitbox, int16_t map_x, int16_t map_y) {}
    virtual void on_hit_sprite(const mgc_hitbox &hitbox, const mgc_sprite &opponent) {}
};

bool check_hit_tilemap(ActorIf *actor, mgc_maphit &maphit, const mgc_tilemap &tilemap);
bool check_hit_sprite(ActorIf *actor, mgc_sprhit &sprhit, const mgc_sprite &opp_spr);

}
#endif/*MGC_ACTOR_IF_HPP*/
