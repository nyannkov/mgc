#include "resources/mml/mml.h"
#include "event_stage_1_4.hpp"


namespace app {

using app::event::EventState;

Event_Stage1_4::Event_Stage1_4(
    SceneContext& scx,
    SceneObjects_Stage1_4& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    player_(scx.player),
    objs_(objs) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);

}

void Event_Stage1_4::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::NotStarted);
    unlock_control();
    screen_fader_.clear();
}

void Event_Stage1_4::spawn() {
    spawn({0, 0});
}

void Event_Stage1_4::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage1_4::update() {

    switch (event_state()) {
    case EventState::NotStarted:
        for ( auto& teleporter : objs_.teleporters() ) {
            if ( teleporter.try_entry() ) {
                active_teleporter_ = &teleporter;
                set_event_state(EventState::Playing);
                screen_fader_.request_fade_out();
                lock_control();
            }
        }
        break;

    case EventState::Playing:
        if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
            player_.set_position(active_teleporter_->destination());
            screen_fader_.request_fade_in();
        } else if ( screen_fader_.fade_state() == ScreenFaderState::FadeInComplete ) {
            set_event_state(EventState::Finished);
        } else { }
        break;

    case EventState::Finished:
        active_teleporter_->reset_entry();
        screen_fader_.clear();
        set_event_state(EventState::NotStarted);
        unlock_control();
        break;
    }
}

void Event_Stage1_4::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
    if ( event_state() == EventState::Playing ) {
        screen_fader_.update(fb);
    }
}

} // namespace app

