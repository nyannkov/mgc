/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_MIXINS_WITH_COLLISION_MAP_HPP
#define MGC_ENTITIES_MIXINS_WITH_COLLISION_MAP_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/collision_map.hpp"

namespace mgc {
namespace entities {
namespace mixins {

template <typename Derived>
struct WithCollisionMap {

    void set_collision_map(const mgc::collision::CollisionMap& map) {
        static_cast<Derived*>(this)->set_collision_map_impl();
    }

    const mgc::collision::CollisionMap * get_collision_map() const {
        return static_cast<const Derived*>(this)->get_collision_map_impl();
    }

    void set_collision_enabled(bool enabled) {
        static_cast<Derived*>(this)->set_collision_enabled_impl(enabled);
    }

    bool get_collision_enabled() const {
        return static_cast<const Derived*>(this)->get_collision_enabled_impl();
    }
};

}// namespace mixins
}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_MIXINS_WITH_COLLISION_MAP_HPP*/
