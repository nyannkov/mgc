#include "resources/mml/mml.h"
#include "event_stage_1_3.hpp"

namespace app {

using app::event::EventState;

Event_Stage1_3::Event_Stage1_3(
    SceneContext& scx,
    SceneObjects_Stage1_3& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    locked_gate_(objs.locked_gate()),
    button_(objs.button()) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage1_3::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    unlock_control();
}

void Event_Stage1_3::spawn() {
    spawn({0, 0});
}

void Event_Stage1_3::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage1_3::update() {

    switch (event_state()) {
    case EventState::Playing:
        if ( button_.is_pushed() ) {
            if ( !locked_gate_.is_opened() ) {
                set_event_state(EventState::Finished);
            }
        } 
        break;

    case EventState::Finished:
        locked_gate_.open();
        sound_.play_sound_effect(MML_SE_8_OPEN);
        set_event_state(EventState::NotStarted);
        break;

    default:
        break;
    }
}

} // namespace app

