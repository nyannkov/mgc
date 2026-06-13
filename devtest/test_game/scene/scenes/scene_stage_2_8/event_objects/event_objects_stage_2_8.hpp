#ifndef MGC_EVENTS_OBJECTS_STAGE_2_8_HPP
#define MGC_EVENTS_OBJECTS_STAGE_2_8_HPP

#include "scene/interface/ievent_objects.hpp"
#include "event_stage_2_8.hpp"
#include "event_stage_2_8_appear_enemy.hpp"

namespace app {

struct EventObjects_Stage2_8 : IEventObjects {

    EventObjects_Stage2_8(SceneContext& scx, SceneObjects_Stage2_8& objs)
        : cp_info_(scx.world_state.checkpoint_info),
          event_1_(scx, objs),
          event_2_(scx, objs),
          events_ { &event_1_, &event_2_ } {
    }
    
    void init() override {
        event_1_.spawn();
        event_2_.spawn();
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
    Event_Stage2_8_AppearEnemy event_2_;
    std::array<event::Event*, 2> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_2_8_HPP*/

