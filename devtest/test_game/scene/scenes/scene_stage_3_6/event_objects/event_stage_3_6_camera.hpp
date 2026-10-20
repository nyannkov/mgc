#ifndef MGC_EVENT_STAGE_3_6_CAMERA_HPP
#define MGC_EVENT_STAGE_3_6_CAMERA_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "entity/event/visit/visit.hpp"
#include "scene_objects/scene_objects_stage_3_6.hpp"

namespace app {

struct Event_Stage3_6_Camera : event::Event {

    Event_Stage3_6_Camera(
        SceneContext& scx,
        SceneObjects_Stage3_6& objs,
        CameraT& camera
    ) : player_(scx.player),
        camera_(camera) {
        this->mut_sprite().set_visible(false);
        auto& hitboxes = this->mut_hitboxes();
        hitboxes[0].set_enabled(false);
        set_event_state(event::EventState::NotStarted);
    }

    void spawn(const mgc::math::Vec2i& pos) override {
        camera_.set_target_offset({MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0)});
        unlock_control();
    }
    void spawn() {
        spawn({0, 0});
    }
    void despawn() override {
        set_event_state(event::EventState::NotStarted);
    }
    void update() override {
        
        if ( player_.position().y <= MGC_CELL2PIXEL(17) ) {
            camera_.set_target_offset({MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(-4)});
        } else if ( player_.position().y >= MGC_CELL2PIXEL(18) ) {
            camera_.set_target_offset({MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0)});
        } else { }
        
    }

private:
    Player& player_;
    CameraT& camera_;
};

}// namespace app

#endif/*MGC_EVENT_STAGE_3_6_CAMERA_HPP*/

