#include "warp.hpp"

namespace app {

using app::event::EventState;

Warp::Warp(prop::Portal& portal) : portal_(portal) {
    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Warp::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::NotStarted);
    unlock_control();
    screen_fader_.clear();
    screen_fader_.set_fade_speed(5);
    screen_fader_.set_fill_color(MGC_COLOR_WHITE);
}

void Warp::spawn() {
    spawn({0, 0});
}

void Warp::despawn() {
    set_event_state(EventState::NotStarted);
}

void Warp::update() {

    switch ( event_state() ) {
    case EventState::NotStarted:
        if ( portal_.try_entry() ) {
            set_event_state(EventState::Playing);
            screen_fader_.request_fade_out(ScreenFaderType::Fade);
            lock_control();
        }
        break;
    case EventState::Playing:
        if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
            set_event_state(EventState::Finished);
        }
        break;
    case EventState::Finished:
        portal_.request_scene_change({dest_id_});
        break;
    default:
        break;
    }
}

void Warp::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
    screen_fader_.update(fb);
}

} // namespace app

