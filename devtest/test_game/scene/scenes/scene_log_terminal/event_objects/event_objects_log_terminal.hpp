#ifndef MGC_EVENTS_OBJECTS_LOG_TERMINAL_HPP
#define MGC_EVENTS_OBJECTS_LOG_TERMINAL_HPP

#include "event_log_terminal_1.hpp"
#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_log_terminal.hpp"

namespace app {

struct EventObjects_LogTerminal : IEventObjects {

    EventObjects_LogTerminal(SceneContext& scx, SceneObjects_LogTerminal& objs)
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

    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {}

private:
    CheckpointInfo& cp_info_;
    Event_LogTerminal1 event_1_;
    std::array<event::Event*, 1> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_LOG_TERMINAL_HPP*/

