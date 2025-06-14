/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_ACTOR_IMPL_HPP
#define MGC_ENTITIES_ACTOR_IMPL_HPP

#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/entities/mixins/with_hitboxes.hpp"
#include "mgc_cpp/entities/mixins/with_on_hit_hitbox_response.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/positionable.hpp"

namespace mgc {
namespace entities {


template <typename Derived, size_t MaxHitboxCount>
struct ActorImpl
    : mgc::entities::mixins::WithHitboxes<Derived, MaxHitboxCount>,
      mgc::entities::mixins::WithOnHitHitboxResponse<Derived>,
      mgc::features::Resettable,
      mgc::features::Positionable {

    using SpriteT = mgc::parts::BasicSprite;
    using Hitboxes = std::array<mgc::collision::Hitbox, MaxHitboxCount>; 
    static constexpr size_t HitboxCount = MaxHitboxCount;

    ActorImpl() { reset(); }
    ~ActorImpl() = default;

    const SpriteT& sprite() const { return sprite_; }

    // [feature] Resettable
    void reset() {
        sprite_.reset();
        hitboxes_.fill(mgc::collision::Hitbox{});
    }

    // [feature] Positionable
    using mgc::features::Positionable::set_position;
    mgc::geometry::Point get_position() const override {
        return sprite_.get_position();
    }

    void set_position(int16_t x, int16_t y) override {
        sprite_.set_position(x, y);
    }

    // [impl] WithHitboxes
    const Hitboxes& hitboxes_impl() const {
        return hitboxes_;
    }

    Hitboxes& hitboxes_impl() {
        return hitboxes_;
    }

    const mgc::collision::Hitbox* get_hitbox_by_id_impl(mgc_id_t hitbox_id) const {
        for (const auto& h : hitboxes_) {
            if (h.id == hitbox_id) return &h;
        }
        return nullptr;
    }

    mgc::collision::Hitbox* get_hitbox_by_id_impl(mgc_id_t hitbox_id) {
        for (auto& h : hitboxes_) {
            if (h.id == hitbox_id) return &h;
        }
        return nullptr;
    }

    size_t hitbox_count_impl() const {
        return HitboxCount;
    }

    // [impl] WithOnHitHitboxResponse
    // Users can override `on_hit_hitbox_impl<Other>` in a derived class
    // to customize behavior based on the type of the collided object.
    // You can use `if constexpr` and `std::is_same_v` to dispatch behavior
    // depending on the type statically.
    //
    // Example:
    //
    // template <typename Other>
    // void on_hit_hitbox_impl(const Other& other) {
    //     if constexpr (std::is_same_v<Other, Player>) {
    //         // Handle collision with Player
    //     } else if constexpr (std::is_same_v<Other, Enemy>) {
    //         // Handle collision with Enemy
    //     } else {
    //         // Handle unknown or unsupported type
    //     }
    // }
    //
    // This default implementation does nothing.
    // Users are encouraged to override it.
    template <typename Other>
    void on_hit_hitbox_impl(const Other& other) {
        // No-op by default
    }

protected:
    SpriteT sprite_;
    Hitboxes hitboxes_;
};


}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_ACTOR_IMPL_HPP*/
