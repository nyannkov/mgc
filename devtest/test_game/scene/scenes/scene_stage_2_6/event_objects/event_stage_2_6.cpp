#include "resources/mml/mml.h"
#include "event_stage_2_6.hpp"

namespace app {

using app::event::EventState;

Event_Stage2_6::Event_Stage2_6(
    SceneContext& scx,
    SceneObjects_Stage2_6& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    objs_(objs) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_6::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    unlock_control();

    if ( cp_info_.has_reached(Checkpoint::Puzzle1_Solved) ) {
        objs_.stage().set_water_enabled(true);
    } else {
        objs_.stage().set_water_enabled(false);
    }
}

void Event_Stage2_6::spawn() {
    spawn({0, 0});
}

void Event_Stage2_6::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_6::update() {
}

} // namespace app

