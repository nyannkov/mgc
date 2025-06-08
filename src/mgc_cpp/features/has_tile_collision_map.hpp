/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_HAS_TILE_COLLISION_MAP_HPP
#define MGC_FEATURES_HAS_TILE_COLLISION_MAP_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/assets/tile_collision_map.hpp"

namespace mgc {
namespace features {

struct HasTileCollisionMap {

    virtual ~HasTileCollisionMap() = default;

    virtual void set_tile_collision_map(const mgc::parts::assets::TileCollisionMap& map) = 0;
    virtual const mgc::parts::assets::TileCollisionMap * get_tile_collision_map() const = 0;
    virtual void set_collision_enabled(bool enabled) = 0;
    virtual bool get_collision_enabled() const = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_HAS_TILE_COLLISION_MAP_HPP*/
