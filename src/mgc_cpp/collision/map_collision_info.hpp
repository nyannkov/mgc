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
    size_t obj_hitbox_index;
    uint8_t map_cell_value;
    uint16_t row;
    uint16_t col;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_MAP_COLLISION_INFO_HPP*/
