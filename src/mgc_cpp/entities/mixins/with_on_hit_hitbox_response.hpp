/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_MIXINS_WITH_ON_HIT_HITBOX_RESPONSE_HPP
#define MGC_ENTITIES_MIXINS_WITH_ON_HIT_HITBOX_RESPONSE_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace entities {
namespace mixins {


template <typename Derived>
struct WithOnHitHitboxResponse {
    template <typename Other>
    void on_hit(const Other& other) {
        static_cast<Derived*>(this)->on_hit_hitbox_impl(other);
    }
};


}// namespace mixins
}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_MIXINS_WITH_ON_HIT_RESPONSE_HPP*/
