/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_MAP_PUSHBACK_INFO_HPP
#define MGC_COLLISION_MAP_PUSHBACK_INFO_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/physics/vec2.hpp"

namespace mgc {
namespace collision {

struct MapPushbackInfo {
    const mgc::collision::Hitbox& obj_hitbox;
    mgc::physics::Vec2 pushback;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_MAP_PUSHBACK_INFO_HPP*/
