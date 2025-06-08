/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_HITBOX_ARRAY_HPP
#define MGC_COLLISION_HITBOX_ARRAY_HPP

#include <array>
#include "mgc/components/hitbox.h"
#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace collision {

using Hitbox = mgc_hitbox_t;

template <size_t MaxHitboxCount>
struct HitboxArray {

    HitboxArray() {
        reset();
    }

    ~HitboxArray() = default;

    void reset() {
        for ( auto &h : hitbox_arr_) {
            hitbox_init(&h, 0, 0, 0, 0, 0);
        }
        hitbox_count_ = 0;
    }

    size_t count() const {
        return hitbox_count_;
    }

    bool add_hitbox(mgc_id_t hitbox_id, int8_t x0_ofs, int8_t y0_ofs, uint8_t width, uint8_t height, bool enabled) {
        if ( hitbox_count_ >= MaxHitboxCount ) {
            return false;
        }
        Hitbox *hitbox = hitbox_get_by_id(hitbox_arr_.data(), hitbox_count_, hitbox_id);
        if ( hitbox != nullptr ) {
            return false;
        } else {
            hitbox_init(&hitbox_arr_[hitbox_count_], x0_ofs, y0_ofs, width, height);
            hitbox_set_enabled(&hitbox_arr_[hitbox_count_], enabled);
            hitbox_count_++;
            return true;
        }
    }

    bool add_hitbox(mgc_id_t hitbox_id, const Hitbox &hitbox) {
        return add_hitbox(hitbox_id, hitbox.id, hitbox.x0_ofs, hitbox.y0_ofs, hitbox.width, hitbox.height);
    }

    bool remove_hitbox(mgc_id_t hitbox_id) {
        bool find = false;
        size_t target_index = 0;

        if ( hitbox_count_ == 0 ) {
            return false;
        }
        for ( target_index = 0; target_index < hitbox_count_; target_index++ ) {
            if ( hitbox_id == hitbox_arr_[target_index].id ) {
                find = true;
                break;
            }
        }
        if ( find ) {
            for ( size_t index = target_index; index < (hitbox_count_-1); index++ ) {
                hitbox_arr_[index] = hitbox_arr_[index+1];
            }
            hitbox_count_--;
            return true;
        } else {
            return false;
        }
    }

    bool is_enabled(mgc_id_t hitbox_id) const {
        Hitbox *hitbox = hitbox_get_by_id(hitbox_arr_.data(), hitbox_count_, hitbox_id);
        if ( hitbox != nullptr ) {
            return hitbox_is_enabled(hitbox);
        } else {
            return false;
        }
    }

    void set_enabled(mgc_id_t hitbox_id, bool enabled) {
        Hitbox *hitbox = hitbox_get_by_id(hitbox_arr_.data(), hitbox_count_, hitbox_id);
        if ( hitbox != nullptr ) {
            hitbox_set_enabled(hitbox, enabled);
        }
    }

    const Hitbox *raw() const {
        return hitbox_arr_.empty() ? nullptr : hitbox_arr_.data();
    }

    const Hitbox* get(size_t index) const {
        if ( index < hitbox_count_ ) {
            return &hitbox_arr_[index];
        } else {
            return nullptr;
        }
    }

    const Hitbox* get_by_id(mgc_id_t hitbox_id) const {
        return hitbox_get_by_id(hitbox_arr_.data(), hitbox_count_, hitbox_id);
    }

private:
    std::array<Hitbox, MaxHitboxCount> hitbox_arr_;
    size_t hitbox_count_;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_HITBOX_ARRAY_HPP*/
