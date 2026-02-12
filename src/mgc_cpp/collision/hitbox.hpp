/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_HITBOX_HPP
#define MGC_COLLISION_HITBOX_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"

namespace mgc {
namespace collision {

using HitboxOffset = mgc::math::Vec2<mgc_world_t>;
struct HitboxSize {
    uint16_t width;
    uint16_t height;
};

struct Hitbox {
    Hitbox() {
        hitbox_init(&hitbox_, 0, 0, 0, 0, 0);
    }
    Hitbox(mgc_id_t id, bool enabled, HitboxOffset offset, HitboxSize size) {
        hitbox_init(&hitbox_, id, offset.x, offset.y, size.width, size.height);
    }
    mgc_id_t id() const { return hitbox_.id; }
    HitboxOffset offset() const { return {hitbox_.x0_ofs, hitbox_.y0_ofs}; }
    void set_offset(const HitboxOffset& offset) {
        hitbox_.x0_ofs = offset.x;
        hitbox_.y0_ofs = offset.y;
    }
    HitboxSize size() const { return {hitbox_.width, hitbox_.height}; }
    void set_size(const HitboxSize& size) {
        hitbox_.width = size.width;
        hitbox_.height = size.height;
    }
    bool enabled() const { return hitbox_.enabled; }
    void set_enabled(bool v) {
        hitbox_.enabled = v;
    }
    const mgc_hitbox_t *c_ptr() const {
        return &hitbox_;
    }

private:
    mgc_hitbox_t hitbox_;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_HITBOX_HPP*/
