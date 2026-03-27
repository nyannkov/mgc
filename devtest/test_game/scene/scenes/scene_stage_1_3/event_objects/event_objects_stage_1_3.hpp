#ifndef MGC_EVENTS_OBJECTS_STAGE_1_3_HPP
#define MGC_EVENTS_OBJECTS_STAGE_1_3_HPP

#include "event_stage_1_3.hpp"
#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_corridor.hpp"
#include "entity/event/warp/warp.hpp"

namespace app {

struct EventObjects_Stage1_3 : IEventObjects {

    EventObjects_Stage1_3(SceneContext& scx, SceneObjects_Stage1_3& objs)
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

    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override { }

private:
    CheckpointInfo& cp_info_;
    Event_Stage1_3 event_1_;
    std::array<event::Event*, 1> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_1_3_HPP*/

