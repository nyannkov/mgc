/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_BASIC_TILEMAP_HPP
#define MGC_ENTITIES_BASIC_TILEMAP_HPP

#include "mgc_cpp/collision/collision_map.hpp"
#include "mgc_cpp/features/has_collision_map.hpp"

namespace mgc {
namespace entities {

struct BasicTilemap : mgc::features::Resettable,
                      mgc::features::HasCollisionMap {

    using TilegridT = mgc::parts::BasicTilegrid;

    BasicTilemap() { reset(); }
    ~BasicTilemap() = default;

    // [features] Resettable
    void reset() {
        tilegrid_.reset();
        collision_map_ = nullptr;
        hit_enabled_ = true;
    }
    
    // [features] HasCollisionMap
    void set_tile_collision_map(const mgc::collision::CollisionMap& map) override {
        collision_map_ = &map;
    }

    const mgc::collision::CollisionMap * get_tile_collision_map() const override {
        return collision_map_;
    }

    void set_collision_enabled(bool enabled) override {
        hit_enabled_ = enabled;
    }

    bool get_collision_enabled() const override {
        return hit_enabled_;
    }
 
private:
    TilegridT tilegrid_;
    const mgc::collision::CollisionMap* collision_map_;
    bool hit_enabled_;
};

}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_BASIC_ACTOR_HPP*/
