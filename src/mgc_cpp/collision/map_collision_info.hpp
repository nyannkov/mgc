/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_MAP_COLLISION_INFO_HPP
#define MGC_COLLISION_MAP_COLLISION_INFO_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/collision/collision_map.hpp"

namespace mgc {
namespace collision {

struct MapCollisionInfo {
    const mgc::collision::Hitbox& obj_hitbox;
    uint8_t tile_id;
    uint16_t row;
    uint16_t col;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_MAP_COLLISION_INFO_HPP*/
