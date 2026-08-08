#ifndef MGC_EVENTS_OBJECTS_STAGE_1_4_HPP
#define MGC_EVENTS_OBJECTS_STAGE_1_4_HPP

#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_corridor.hpp"
#include "entity/event/warp/warp.hpp"
#include "entity/event/teleport/teleport.hpp"

namespace app {

struct EventObjects_Stage1_4 : IEventObjects {

    EventObjects_Stage1_4(
        SceneContext& scx,
        SceneObjects_Stage1_4& objs,
        CameraT& camera
    ) : cp_info_(scx.world_state.checkpoint_info),
          camera_(camera),
          warp_(objs.portal()),
          teleport_(
            objs.teleporters().data(),
            objs.teleporters().size(),
            scx.player
          ),
          events_ { &warp_, &teleport_ } {
    }
    
    void init() override {
        teleport_.spawn();
        teleport_.set_camera(&camera_);
        warp_.spawn();
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
    Warp warp_;
    event::Teleport teleport_;
    std::array<event::Event*, 2> events_;
    CameraT& camera_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_1_4_HPP*/

