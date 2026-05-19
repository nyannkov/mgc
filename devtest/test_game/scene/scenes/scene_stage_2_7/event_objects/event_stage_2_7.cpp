#include "resources/mml/mml.h"
#include "event_stage_2_7.hpp"

namespace app {

using app::event::EventState;

Event_Stage2_7::Event_Stage2_7(
    SceneContext& scx,
    SceneObjects_Stage2_7& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    objs_(objs) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_7::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    unlock_control();

    objs_.stage().set_water_enabled(true);

    wait_flag_ = false;
}

void Event_Stage2_7::spawn() {
    spawn({0, 0});
}

void Event_Stage2_7::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_7::update() {
    switch (event_state()) {
    case EventState::Playing:
        if ( objs_.button().is_pushed() ) {

            auto pos = objs_.stage().water_position();
            pos.y += 8;
            objs_.stage().set_water_position(pos);

            if ( pos.y >= MGC_CELL2PIXEL(6) ) {
                set_event_state(EventState::Finished);
            }
        } 
        break;

    case EventState::Finished:
        set_event_state(EventState::NotStarted);
        break;

    default:
        break;
    }
}

} // namespace app

