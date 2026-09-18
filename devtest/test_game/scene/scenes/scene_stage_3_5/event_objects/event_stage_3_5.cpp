#include "resources/mml/mml.h"
#include "event_stage_3_5.hpp"

namespace app {

using app::event::EventState;

Event_Stage3_5::Event_Stage3_5(
    SceneContext& scx,
    SceneObjects_Stage3_5& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    button_(objs.button()),
    carrier_(objs.carrier()) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage3_5::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    unlock_control();
}

void Event_Stage3_5::spawn() {
    spawn({0, 0});
}

void Event_Stage3_5::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage3_5::update() {

    switch (event_state()) {
    case EventState::Playing:

        if ( carrier_.state() == carrier::WaypointCarrierState::Finished ) {
            if ( button_.is_pushed() ) {
                carrier_.start(wp_button_on_.data(), wp_button_on_.size(), carrier::WaypointCarrierMode::OneWay);
            } else {
                carrier_.start(wp_button_off_.data(), wp_button_off_.size(), carrier::WaypointCarrierMode::OneWay);
            }
            set_event_state(EventState::Finished);
        }
        break;

    default:
        break;
    }
}

} // namespace app

