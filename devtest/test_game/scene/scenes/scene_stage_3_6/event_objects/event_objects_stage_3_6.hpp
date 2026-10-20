#ifndef MGC_EVENTS_OBJECTS_STAGE_3_6_HPP
#define MGC_EVENTS_OBJECTS_STAGE_3_6_HPP

#include "event_stage_3_6.hpp"
#include "event_stage_3_6_camera.hpp"
#include "scene/interface/ievent_objects.hpp"
#include "entity/event/visit/visit.hpp"

namespace app {

struct EventObjects_Stage3_6 : IEventObjects {

    EventObjects_Stage3_6(
        SceneContext& scx,
        SceneObjects_Stage3_6& objs,
        CameraT& camera
    ) : cp_info_(scx.world_state.checkpoint_info),
        visit_ {
            Visit(scx.timer),
            Visit(scx.timer),
            Visit(scx.timer),
            Visit(scx.timer),
            Visit(scx.timer),
            Visit(scx.timer),
            Visit(scx.timer)
        },
        event_1_(scx, objs, camera, visit_),
        event_2_(scx, objs, camera),
        events_ { 
            &visit_[0],
            &visit_[1],
            &visit_[2],
            &visit_[3],
            &visit_[4],
            &visit_[5],
            &visit_[6],
            &event_1_,
            &event_2_
        } {
    }
    
    void init() override {
        if ( !cp_info_.has_reached(Checkpoint::WonTheStage3Boss) ) {
            event_1_.spawn();
        }
        event_2_.spawn();
    }

    ArrayViewer<event::Event*> events() override {
        return { events_.data(), events_.size() };
    }

//    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {
//        for ( auto* evt : events_ ) {
//            if ( evt ) {
//                evt->draw_effect(fb, cam_pos);
//            }
//        }
//    }

private:
    CheckpointInfo& cp_info_;
    VisitArrayT  visit_;
    Event_Stage3_6 event_1_;
    Event_Stage3_6_Camera event_2_;
    std::array<event::Event*, 9> events_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_STAGE_3_6_HPP*/

