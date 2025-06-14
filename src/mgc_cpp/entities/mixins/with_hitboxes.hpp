/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_MIXINS_WITH_HITBOXES_HPP
#define MGC_ENTITIES_MIXINS_WITH_HITBOXES_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/hitbox.hpp"

namespace mgc {
namespace entities {
namespace mixins {


template <typename Derived, size_t MaxHitboxCount>
struct WithHitboxes {
    using Hitboxes = std::array<mgc::collision::Hitbox, MaxHitboxCount>; 

    const Hitboxes& hitboxes() const {
        return static_cast<const Derived*>(this)->hitboxes_impl();
    }

    Hitboxes& hitboxes() {
        return static_cast<Derived*>(this)->hitboxes_impl();
    }

    const mgc::collision::Hitbox* get_hitbox_by_id(mgc_id_t hitbox_id) const {
        return static_cast<Derived*>(this)->get_hitbox_by_id_impl(hitbox_id);
    }

    mgc::collision::Hitbox* get_hitbox_by_id(mgc_id_t hitbox_id) {
        return static_cast<Derived*>(this)->get_hitbox_by_id_impl(hitbox_id);
    }

    size_t hitbox_count() const {
        return static_cast<Derived*>(this)->hitbox_count_impl();
    }
};

}// namespace mixins
}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_MIXINS_WITH_HITBOXES_HPP*/
