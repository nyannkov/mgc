#ifndef MGC_EVENTS_OBJECTS_CORRIDOR_HPP
#define MGC_EVENTS_OBJECTS_CORRIDOR_HPP

#include "event_corridor_1.hpp"
#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_corridor.hpp"

namespace app {

struct EventObjects_Corridor : IEventObjects {

    EventObjects_Corridor(SceneContext& scx, SceneObjects_Corridor& objs)
        : cp_info_(scx.checkpoint_info),
          event_1_(scx, objs),
          events_ { &event_1_ } {
    }
    
    void init() override {
        if ( !cp_info_.has_reached(Checkpoint::Corridor_OpenedTheDoor) ) {
            event_1_.spawn({MGC_CELL2PIXEL(53), MGC_CELL2PIXEL(0)});
        }
    }

    ArrayViewer<event::Event*> events() override {
        return { events_.data(), events_.size() };
    }

private:
    CheckpointInfo& cp_info_;
    Event_Corridor1 event_1_;
    std::array<event::Event*, 1> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_CORRIDOR_HPP*/

