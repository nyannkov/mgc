/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_HAS_HITBOX_HPP
#define MGC_FEATURES_HAS_HITBOX_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/hitbox_array.hpp"

namespace mgc {
namespace features {

struct HasHitbox {

    virtual ~HasHitbox() = default;

    virtual const collision::Hitbox* get_hitbox(size_t index) const = 0;
    virtual const collision::Hitbox* get_hitbox_by_id(mgc_id_t hitbox_id) const = 0;
    virtual size_t hitbox_count() const = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_HAS_HITBOX_HPP*/
