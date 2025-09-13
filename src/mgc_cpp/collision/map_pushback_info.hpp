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
#include "mgc_cpp/math/vec2.hpp"

namespace mgc {
namespace collision {

struct MapPushbackInfo {
    const mgc::collision::Hitbox& obj_hitbox;
    size_t obj_hitbox_index;
    mgc::math::Vec2i pushback;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_MAP_PUSHBACK_INFO_HPP*/
