#ifndef MGC_EVENTS_OBJECTS_STAGE_1_4_HPP
#define MGC_EVENTS_OBJECTS_STAGE_1_4_HPP

#include "event_stage_1_4.hpp"
#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_corridor.hpp"
#include "entity/event/warp/warp.hpp"

namespace app {

struct EventObjects_Stage1_4 : IEventObjects {

    EventObjects_Stage1_4(SceneContext& scx, SceneObjects_Stage1_4& objs)
        : cp_info_(scx.world_state.checkpoint_info),
          event_1_(scx, objs),
          warp_(objs.portal()),
          events_ { &event_1_, &warp_ } {
    }
    
    void init() override {
        event_1_.spawn();
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
    Event_Stage1_4 event_1_;
    Warp warp_;
    std::array<event::Event*, 2> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_1_4_HPP*/

