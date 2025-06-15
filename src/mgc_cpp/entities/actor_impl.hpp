/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_ACTOR_IMPL_HPP
#define MGC_ENTITIES_ACTOR_IMPL_HPP

#include <array>
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/entities/mixins/with_hitboxes.hpp"
#include "mgc_cpp/entities/mixins/with_on_hit_box_to_box_response.hpp"
#include "mgc_cpp/entities/mixins/with_on_hit_box_to_map_response.hpp"
#include "mgc_cpp/entities/mixins/with_handle_map_pushback_result.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/positionable.hpp"

namespace mgc {
namespace entities {


template <typename Derived, size_t MaxHitboxCount>
struct ActorImpl
    : mgc::entities::mixins::WithHitboxes<Derived, MaxHitboxCount>,
      mgc::entities::mixins::WithOnHitBoxToBoxResponse<Derived>,
      mgc::entities::mixins::WithOnHitBoxToMapResponse<Derived>,
      mgc::entities::mixins::WithHandleMapPushbackResult<Derived>,
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

    // [impl] WithOnHitBoxToBoxResponse
    // Users can override this template method `on_hit_box_to_box_impl<Other>`
    // in their derived class to customize behavior depending on the type `Other`
    // that the object collided with.
    //
    // You can use `if constexpr` along with `std::is_same_v` to implement
    // static dispatch based on the type.
    //
    // Example:
    //
    // template <typename Other>
    // void on_hit_box_to_box_impl(
    //     const Other& other,
    //     const mgc::collision::BoxCollisionInfo& info
    // ) {
    //     if constexpr (std::is_same_v<Other, Player>) {
    //         // Handle collision with Player
    //     } else if constexpr (std::is_same_v<Other, Enemy>) {
    //         // Handle collision with Enemy
    //     } else {
    //         // Fallback for unknown or unsupported types
    //     }
    // }
    //
    // The default implementation does nothing.
    // Override as needed.
    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { }


    // [impl] WithOnHitBoxToMapResponse
    // Called when a collision occurs between an object and the map.
    // Receives the object `obj`, the map `map`, and collision info `info`.
    // The default implementation does nothing.
    // Override this method to handle object-map collisions.
    template <typename ObjT, typename MapT>
    void on_hit_box_to_map_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapCollisionInfo& info
    ) { }


    // [impl] WithPostHitBoxToMapResponse
    // Called after a pushback correction has been applied due to map collision.
    // Receives the object `obj`, the map `map`, and the pushback result `info`.
    // Override this method to perform post-processing after position adjustment.
    // The default implementation does nothing.
    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { }

protected:
    SpriteT sprite_;
    Hitboxes hitboxes_;
};


}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_ACTOR_IMPL_HPP*/
