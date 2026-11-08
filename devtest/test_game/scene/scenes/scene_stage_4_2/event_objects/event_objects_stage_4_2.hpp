#ifndef MGC_EVENTS_OBJECTS_STAGE_4_2_HPP
#define MGC_EVENTS_OBJECTS_STAGE_4_2_HPP

#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_corridor.hpp"
#include "entity/event/warp/warp.hpp"
#include "entity/event/teleport/teleport.hpp"
#include "entity/event/light_effect/light_effect.hpp"
#include "world_state/equipment_info.hpp"

namespace app {

struct EventObjects_Stage4_2 : IEventObjects {

    EventObjects_Stage4_2(
        SceneContext& scx,
        SceneObjects_Stage4_2& objs,
        CameraT& camera
    ) : cp_info_(scx.world_state.checkpoint_info),
        equipment_info_(scx.world_state.equipment_info),
        camera_(camera),
        warp_(objs.portal()),
        light_effect_(scx.player),
        teleport_(
            objs.teleporters().data(),
            objs.teleporters().size(),
            scx.player
        ),
        events_ { &warp_, &teleport_, &light_effect_ } {
    }
    
    void init() override {
        teleport_.spawn();
        teleport_.set_camera(&camera_);
        warp_.spawn();
        light_effect_.spawn();
        light_effect_.set_wide_mode(
            equipment_info_.item.has_item_at(ItemId::Lantern)
        );

        warp_.set_destination(SceneId::Shop);
    }

    ArrayViewer<event::Event*> events() override {
        return { events_.data(), events_.size() };
    }

    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {
        for ( auto* evt : events_ ) {
            if ( evt ) {
                evt->draw_effect(fb, cam_pos);
            }
        }
    }

private:
    CheckpointInfo& cp_info_;
    const EquipmentInfo& equipment_info_;
    Warp warp_;
    event::Teleport teleport_;
    event::LightEffect light_effect_;
    std::array<event::Event*, 3> events_;
    CameraT& camera_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_4_2_HPP*/

