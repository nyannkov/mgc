#ifndef MGC_EVENTS_OBJECTS_TOWER_FRONT_HPP
#define MGC_EVENTS_OBJECTS_TOWER_FRONT_HPP

#include "event_tower_front_1.hpp"
#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_tower_front.hpp"

namespace app {

struct EventObjects_TowerFront : IEventObjects {

    EventObjects_TowerFront(SceneContext& scx, SceneObjects_TowerFront& objs)
        : cp_info_(scx.world_state.checkpoint_info),
          event_1_(scx, objs),
          events_ { &event_1_ } {
    }
    
    void init() override {
        if ( !cp_info_.has_reached(Checkpoint::TowerFrontEvent_Finished) ) {
            event_1_.spawn({MGC_CELL2PIXEL(53), MGC_CELL2PIXEL(0)});
        }
    }

    ArrayViewer<event::Event*> events() override {
        return { events_.data(), events_.size() };
    }
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {}

private:
    CheckpointInfo& cp_info_;
    Event_TowerFront1 event_1_;
    std::array<event::Event*, 1> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_TOWER_FRONT_HPP*/

