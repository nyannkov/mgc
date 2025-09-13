/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_ACTOR_IMPL_HPP
#define MGC_ENTITIES_ACTOR_IMPL_HPP

#include <array>
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/entities/mixins/with_hitboxes.hpp"
#include "mgc_cpp/entities/mixins/with_collision_map.hpp"
#include "mgc_cpp/entities/mixins/with_on_hit_box_to_box_response.hpp"
#include "mgc_cpp/entities/mixins/with_on_hit_box_to_map_response.hpp"
#include "mgc_cpp/entities/mixins/with_handle_map_pushback_result.hpp"
#include "mgc_cpp/collision/collision_detector.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/has_position.hpp"

namespace mgc {
namespace entities {


template <typename Derived, size_t MaxHitboxCount>
struct ActorImpl
    : mgc::entities::mixins::WithHitboxes<Derived, MaxHitboxCount>,
      mgc::entities::mixins::WithOnHitBoxToBoxResponse<Derived>,
      mgc::entities::mixins::WithOnHitBoxToMapResponse<Derived>,
      mgc::entities::mixins::WithHandleMapPushbackResult<Derived>,
      mgc::features::HasId,
      mgc::features::HasPosition<mgc::math::Vec2i>,
      mgc::features::Visible,
      mgc::features::Drawable,
      mgc::features::CellDrawable {

    friend mgc::collision::CollisionDetectorBoxToBox;
    friend mgc::collision::CollisionDetectorBoxToMap;

    using SpriteT = mgc::parts::BasicSprite;
    using Hitboxes = std::array<mgc::collision::Hitbox, MaxHitboxCount>; 
    static constexpr size_t HitboxCount = MaxHitboxCount;

    ActorImpl() : id_(0), real_pos_(0.0f, 0.0f) {
        sprite_.reset();
        sprite_.set_position(real_pos_.template cast_to<int16_t>());
        for ( auto& h : hitboxes_ ) {
            h.enabled = false;
        }
    }
    ~ActorImpl() = default;
    ActorImpl(const ActorImpl&) = delete;
    ActorImpl& operator=(const ActorImpl&) = delete;
    ActorImpl(ActorImpl&&) = default;
    ActorImpl& operator=(ActorImpl&&) = default;

    // [features] HasId
    void set_id(mgc_id_t id) override { id_ = id; }
    mgc_id_t id() const override { return id_; }

    // [feature] HasPosition
    mgc::math::Vec2i position() const override {
        return sprite_.position();
    }

    void set_position(const mgc::math::Vec2i& position) override {
        sprite_.set_position(position);
        real_pos_ = position.template cast_to<float>();
    }

    // HasPosition-related
    mgc::math::Vec2f precise_position() const {
        return real_pos_;
    }

    void set_precise_position(const mgc::math::Vec2f& real_position) {
        real_pos_ = real_position;
        sprite_.set_position(real_pos_.template cast_to<int16_t>());
    }

    // [feature] Visible
    bool is_visible() const override {
        return sprite_.is_visible();
    }

    void set_visible(bool v) override {
        sprite_.set_visible(v);
    }

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override {
        return sprite_.draw(fb, cam_pos, options);
    }

    // [feature] CellDrawable
    using mgc::features::CellDrawable::cell_draw;
    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override {
        return sprite_.cell_draw(cb, cell_x, cell_y, cam_pos, options);
    }

    // [impl] WithHitboxes
    const Hitboxes& hitboxes_impl() const {
        return hitboxes_;
    }

    const mgc::collision::Hitbox* get_hitbox_by_id_impl(mgc_id_t hitbox_id) const {
        for (const auto& h : hitboxes_) {
            if (h.id == hitbox_id) return &h;
        }
        return nullptr;
    }

    size_t hitbox_count_impl() const {
        return HitboxCount;
    }

    // [impl] WithOnHitBoxToBoxResponse
    // Users can provide their own implementation of `on_hit_box_to_box_impl<Other>`
    // in a derived class to handle collisions with objects of type `Other`.
    //
    // Notes on const correctness:
    // - The collision detector always passes `Other` as a non-const reference.
    // - You may choose to receive it as `const Other&` for read-only access.
    // - If you intend to modify the other object, receive it as `Other&`.
    //
    // Example usage:
    //
    // template <typename Other>
    // void on_hit_box_to_box_impl(const Other& other,
    //                             const mgc::collision::BoxCollisionInfo& info) {
    //     // Read-only access; safe for inspecting the other object.
    // }
    //
    // template <typename Other>
    // void on_hit_box_to_box_impl(Other& other,
    //                             const mgc::collision::BoxCollisionInfo& info) {
    //     // Mutable access; you can modify `other`.
    //     other.take_damage(1);
    // }
    //
    // The default implementation does nothing. Override as needed in your derived class.
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

    const SpriteT& sprite() const { return sprite_; }
    const Hitboxes& hitboxes() const { return hitboxes_; }
    const mgc::collision::Hitbox* get_hitbox_by_id(mgc_id_t hitbox_id) const {
        for (auto& h : hitboxes_) {
            if (h.id == hitbox_id) return &h;
        }
        return nullptr;
    }

protected:
    SpriteT& sprite() { return sprite_; }
    Hitboxes& hitboxes() { return hitboxes_; }
    mgc::collision::Hitbox* get_hitbox_by_id(mgc_id_t hitbox_id) {
        for (auto& h : hitboxes_) {
            if (h.id == hitbox_id) return &h;
        }
        return nullptr;
    }

private:
    mgc_id_t id_;
    SpriteT sprite_;
    mgc::math::Vec2f real_pos_;
    Hitboxes hitboxes_;
};


}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_ACTOR_IMPL_HPP*/
