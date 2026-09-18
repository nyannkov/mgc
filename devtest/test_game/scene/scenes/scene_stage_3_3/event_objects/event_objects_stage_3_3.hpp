#ifndef MGC_EVENTS_OBJECTS_STAGE_3_3_HPP
#define MGC_EVENTS_OBJECTS_STAGE_3_3_HPP

#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_corridor.hpp"
#include "entity/event/warp/warp.hpp"
#include "entity/event/teleport/teleport.hpp"

namespace app {

struct EventObjects_Stage3_3 : IEventObjects {

    EventObjects_Stage3_3(
        SceneContext& scx,
        SceneObjects_Stage3_3& objs,
        CameraT& camera
    ) : cp_info_(scx.world_state.checkpoint_info),
          camera_(camera),
          warp_(objs.portal()),
          events_ { &warp_ } {
    }
    
    void init() override {
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
    std::array<event::Event*, 1> events_;
    CameraT& camera_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_3_3_HPP*/

