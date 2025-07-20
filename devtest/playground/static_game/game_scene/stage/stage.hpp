#ifndef MGC_STAGE_HPP
#define MGC_STAGE_HPP

#include "mgc_cpp/mgc.hpp"
#include "main_layer.hpp"
#include "background_layer.hpp"

namespace app {

// Collision detection currently requires mixin inheritance, which can be inconvenient.
// It may be better to conditionally call pushback_result handlers only if they are 
// implemented, using constexpr-based trait checks within the CollisionDetector.
struct Stage : mgc::entities::mixins::WithCollisionMap<Stage>,
               mgc::entities::mixins::WithOnHitBoxToMapResponse<Stage>,
               mgc::entities::mixins::WithHandleMapPushbackResult<Stage>,
               mgc::features::HasPosition<mgc::math::Vec2i> {

    enum class StageId : mgc_id_t {
        None = 0,
        Stage1
    };

    Stage() { reset(); }
    ~Stage() = default;
    Stage(const Stage&) = delete;
    Stage& operator=(const Stage&) = delete;
    Stage(Stage&&) = default;
    Stage& operator=(Stage&&) = default;

    void reset() {
        main_layer_.setup_by_id(MainLayer::LayerId::None);
        bg_layer_.setup_by_id(BackgroundLayer::LayerId::None);
    }

    void setup_by_id(StageId id) {

        stage_id_ = id;

        switch (id) {
        case StageId::Stage1:
            main_layer_.setup_by_id(MainLayer::LayerId::Layer1);
            bg_layer_.setup_by_id(BackgroundLayer::LayerId::Layer1);
            break;

        case StageId::None:/*fallthrought*/
        default:
            main_layer_.setup_by_id(MainLayer::LayerId::None);
            bg_layer_.setup_by_id(BackgroundLayer::LayerId::None);
            break;
        }
    }

    void update() {
        bg_layer_.update();
    }

    const mgc::features::Drawable* main_layer() const {
        return &main_layer_;
    }

    const mgc::features::Drawable* bg_layer() const {
        return &bg_layer_;
    }

    // [impl] WithCollisionMap
    const mgc::collision::CollisionMap *collision_map_impl() const {
        return main_layer_.collision_map();
    }

    bool collision_enabled_impl() const {
        return main_layer_.collision_enabled_impl();
    }

    // [impl] WithOnHitBoxToMapResponse (for Map side)
    template <typename ObjT, typename MapT>
    void on_hit_box_to_map_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapCollisionInfo& info
    ) { }

    // [impl] WithPostHitBoxToMapResponse (for Map side)
    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { }

    // [features] HasPosition
    mgc::math::Vec2i position() const override {
        return main_layer_.position();
    }

    void set_position(const mgc::math::Vec2i& position) override {
        main_layer_.set_position(position);
        bg_layer_.set_position(position + mgc::math::Vec2i(MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(34)));
    }

private:
    StageId stage_id_;
    MainLayer main_layer_;
    BackgroundLayer bg_layer_;
};

}// namespace app

#endif/*MGC_STAGE_HPP*/

