/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_PLAYER_HPP
#define MGC_PLAYER_HPP

#include "mgc/mgc.h"
#include "mgc/utils/jumpctrl/jumpctrl.h"
#include "actor_if.hpp"
#include "anim/anim_player.hpp"

namespace mgc {

struct Player : ActorIf {
    void init(mgc_id_t id, int16_t x, int16_t y) override;
    void update() override;
    const mgc_sprite *get_ptr_sprite() const override;
    void on_hit_tilemap(const mgc_hitbox &hitbox, int16_t map_x, int16_t map_y) override;
    void set_anim(AnimType type, AnimDir dir);
    void update_anim();
private:
    int16_t mark_x_;
    int16_t mark_y_;
    int8_t vx_;
    int8_t vy_;
    bool foot_hit_;
    mgc_sprite sprite_;
    enum hitbox_id {
        HITBOX_ID_BODY = 0,
        HITBOX_ID_HEAD,
        HITBOX_ID_FOOT,
        NUM_OF_HITBOX_ID
    };
    mgc_hitbox hitbox_array_[NUM_OF_HITBOX_ID];
    mgc_jumpctrl jumpctrl_;
    AnimPlayer anim_player_;
    void set_position(int16_t x, int16_t y);
    void mark_current_position();
};

}
#endif/*MGC_PLAYER_HPP*/
