#include "resources/mml/mml.h"
#include "event_corridor_1.hpp"


namespace app {

using app::event::EventState;

Event_Corridor1::Event_Corridor1(
    SceneContext& scx,
    SceneObjects_Corridor& objs
) : sound_(scx.sound),
    cp_info_(scx.checkpoint_info),
    signboard_(objs.signboard()),
    gate_(objs.gate()) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);

}

void Event_Corridor1::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);

    //TODO
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_SIGNBOARD_1,
        &talkflow_listener_,
        TalkflowEffectType::Mute
    });
}

void Event_Corridor1::spawn() {
    spawn({0, 0});
}

void Event_Corridor1::despawn() {
    set_event_state(EventState::NotStarted);
    talkflow_listener_.clear_checked_count();
}

void Event_Corridor1::update() {
    
    if ( event_state() != EventState::Playing ) {
        return;
    }

    if ( talkflow_listener_.checked_count() > 0 ) {
        sound_.play_sound_effect(MML_SE_8_OPEN, 0.0);
        gate_.open();
        set_event_state(EventState::Finished);
        cp_info_.advance_checkpoint(Checkpoint::Corridor_OpenedTheDoor);
    }
}


} // namespace app

