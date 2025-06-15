/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_MIXINS_WITH_ON_HIT_BOX_TO_BOX_RESPONSE_HPP
#define MGC_ENTITIES_MIXINS_WITH_ON_HIT_BOX_TO_BOX_RESPONSE_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/collision/box_collision_info.hpp"

namespace mgc {
namespace entities {
namespace mixins {

template <typename Derived>
struct WithOnHitBoxToBoxResponse {
    template <typename Other>
    void on_hit_box_to_box(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) {
        static_cast<Derived*>(this)->on_hit_box_to_box_impl(other, info);
    }
};

}// namespace mixins
}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_MIXINS_WITH_ON_HIT_BOX_TO_BOX_RESPONSE_HPP*/
