/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_BOX_PUSHBACK_INFO_HPP
#define MGC_COLLISION_BOX_PUSHBACK_INFO_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/math/vec2.hpp"

namespace mgc {
namespace collision {

struct BoxPushbackInfo {
    mgc::math::Vec2i pushback;
    mgc::math::Vec2i max_overlap;
    bool is_fully_blocked;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_BOX_PUSHBACK_INFO_HPP*/

