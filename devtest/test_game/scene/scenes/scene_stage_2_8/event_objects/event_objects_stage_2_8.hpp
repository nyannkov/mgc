#ifndef MGC_EVENTS_OBJECTS_STAGE_2_8_HPP
#define MGC_EVENTS_OBJECTS_STAGE_2_8_HPP

#include "scene/interface/ievent_objects.hpp"
#include "entity/event/warp/warp.hpp"
#include "event_stage_2_8.hpp"
#include "entity/event/warp/warp.hpp"
#include "entity/event/teleport/teleport.hpp"

namespace app {

struct EventObjects_Stage2_8 : IEventObjects {

    EventObjects_Stage2_8(SceneContext& scx, SceneObjects_Stage2_8& objs)
        : cp_info_(scx.world_state.checkpoint_info),
          event_1_(scx, objs),
          events_ { &event_1_ } {
    }
    
    void init() override {
//        event_1_.spawn();
//        warp_.spawn();
//        warp_.set_destination(SceneId::Shop);
//        teleport_.spawn();
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
    Event_Stage2_8 event_1_;
    std::array<event::Event*, 1> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_2_8_HPP*/

