/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_TILEMAP_IMPL_HPP
#define MGC_ENTITIES_TILEMAP_IMPL_HPP

#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/entities/mixins/with_collision_map.hpp"
#include "mgc_cpp/entities/mixins/with_on_hit_box_to_map_response.hpp"
#include "mgc_cpp/entities/mixins/with_handle_map_pushback_result.hpp"
#include "mgc_cpp/collision/collision_detector.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/has_position.hpp"

namespace mgc {
namespace entities {

template <typename Derived>
struct TilemapImpl : mgc::entities::mixins::WithCollisionMap<Derived>,
                     mgc::entities::mixins::WithOnHitBoxToMapResponse<Derived>,
                     mgc::entities::mixins::WithHandleMapPushbackResult<Derived>,
                     mgc::features::HasId,
                     mgc::features::HasPosition<mgc::math::Vec2i>,
                     mgc::features::Visible,
                     mgc::features::Drawable,
                     mgc::features::CellDrawable {
                      
    friend mgc::collision::CollisionDetectorBoxToMap;
    using TilegridT = mgc::parts::BasicTilegrid;

    TilemapImpl() : id_(0), hit_enabled_(false), collision_map_(nullptr) { 
        tilegrid_.reset();
    }
    ~TilemapImpl() = default;
    TilemapImpl(const TilemapImpl&) = delete;
    TilemapImpl& operator=(const TilemapImpl&) = delete;
    TilemapImpl(TilemapImpl&&) = default;
    TilemapImpl& operator=(TilemapImpl&&) = default;

    // [features] HasId
    void set_id(mgc_id_t id) override { id_ = id; }
    mgc_id_t id() const override { return id_; }

    // [features] HasPosition
    mgc::math::Vec2i position() const override {
        return tilegrid_.position();
    }

    void set_position(const mgc::math::Vec2i& position) override {
        tilegrid_.set_position(position);
    }

    // [feature] Visible
    bool is_visible() const override {
        return tilegrid_.is_visible();
    }

    void set_visible(bool v) override {
        tilegrid_.set_visible(v);
    }

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override {
        return tilegrid_.draw(fb, cam_pos, options);
    }

    // [feature] CellDrawable
    using mgc::features::CellDrawable::cell_draw;
    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override {
        return tilegrid_.cell_draw(cb, cell_x, cell_y, cam_pos, options);
    }
    
    // [impl] WithCollisionMap
    const mgc::collision::CollisionMap * collision_map_impl() const {
        return collision_map_;
    }

    bool collision_enabled_impl() const {
        return hit_enabled_;
    }

    // [impl] WithOnHitBoxToMapResponse (for Map side)
    // This method can be overridden on the *Map* side to handle collisions
    // when an object (e.g., player, enemy) hits the map.
    //
    // Called during collision detection between an object and the map.
    // Receives the object `obj`, the map itself `map`, and collision info `info`. 
    //
    // The default implementation does nothing.
    // Override as needed.
    template <typename ObjT, typename MapT>
    void on_hit_box_to_map_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapCollisionInfo& info
    ) { }

    // [impl] WithPostHitBoxToMapResponse (for Map side)
    // This method is called after a pushback (position correction) has been applied
    // due to a collision between an object and the map.
    //
    // It provides access to the object `obj`, the map `map`, and pushback result `info`.
    //
    // The default implementation does nothing.
    // Override as needed.
    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { }

    const TilegridT& tilegrid() const { return tilegrid_; }

protected:
    TilegridT& tilegrid() { return tilegrid_; }
    void set_collision_map(const mgc::collision::CollisionMap* map) { collision_map_ = map; }
    void set_collision_enabled(bool enabled) { hit_enabled_ = enabled; }

private:
    const mgc::collision::CollisionMap* collision_map_;
    bool hit_enabled_;
    mgc_id_t id_;
    TilegridT tilegrid_;
};

}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_BASIC_ACTOR_HPP*/
