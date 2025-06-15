/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_MIXINS_WITH_ON_HIT_BOX_TO_MAP_RESPONSE_HPP
#define MGC_ENTITIES_MIXINS_WITH_ON_HIT_BOX_TO_MAP_RESPONSE_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/map_collision_info.hpp"

namespace mgc {
namespace entities {
namespace mixins {

template <typename Derived>
struct WithOnHitBoxToMapResponse {
    template <typename ObjT, typename MapT>
    void on_hit_box_to_map(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapCollisionInfo& info
    ) {
        static_cast<Derived*>(this)->on_hit_box_to_map_impl(obj, map, info);
    }
};


}// namespace mixins
}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_MIXINS_WITH_ON_HIT_BOX_TO_MAP_RESPONSE_HPP*/
