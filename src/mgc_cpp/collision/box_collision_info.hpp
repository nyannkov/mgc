/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_BOX_COLLISION_INFO_HPP
#define MGC_COLLISION_BOX_COLLISION_INFO_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/hitbox.hpp"

namespace mgc {
namespace collision {

struct BoxCollisionInfo {
    const mgc::collision::Hitbox& self_hitbox;
    const mgc::collision::Hitbox& other_hitbox;
    size_t self_hitbox_index;
    size_t other_hitbox_index;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_BOX_COLLISION_INFO_HPP*/
