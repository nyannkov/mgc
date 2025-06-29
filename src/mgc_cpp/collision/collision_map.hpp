/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_COLLISION_MAP_HPP
#define MGC_COLLISION_COLLISION_MAP_HPP

#include "mgc/components/map.h"

namespace mgc {
namespace collision {

// Note:
// Currently, CollisionMap and TileIdMap share the same underlying data type (mgc_map_t).
// This is because both tile ID and collision flag are stored in a single 8-bit value:
//   - The upper 1 bit represents collision (1 = collidable, 0 = non-collidable).
//   - The lower 7 bits represent the tile ID.
//
// In the future, this format will be revised to separate collision data from tile ID data.
// At that point, CollisionMap will represent only the collision flags, independent of TileIdMap.
using CollisionMap = mgc_map_t;

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_COLLISION_MAP_HPP*/
