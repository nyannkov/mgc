#ifndef MGC_EVENTS_OBJECTS_STAGE_2_6_HPP
#define MGC_EVENTS_OBJECTS_STAGE_2_6_HPP

#include "scene/interface/ievent_objects.hpp"
#include "entity/event/warp/warp.hpp"
#include "event_stage_2_6.hpp"

namespace app {

struct EventObjects_Stage2_6 : IEventObjects {

    EventObjects_Stage2_6(SceneContext& scx, SceneObjects_Stage2_6& objs)
        : cp_info_(scx.world_state.checkpoint_info),
          event_1_(scx, objs),
          events_ { &event_1_ } {
    }
    
    void init() override {
        event_1_.spawn();
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
    Event_Stage2_6 event_1_;
    std::array<event::Event*, 1> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_2_6_HPP*/

