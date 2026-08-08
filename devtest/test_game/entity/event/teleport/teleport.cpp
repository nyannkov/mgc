#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "teleport.hpp"
#include "entity/player/player.hpp"

namespace app {
namespace event {

Teleport::Teleport(
    prop::Teleporter* teleporters, 
    size_t teleporters_count,
    Player& player
) : teleporters_(teleporters),
    teleporters_count_(teleporters_count),
    player_(player)
{
    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Teleport::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::NotStarted);
    unlock_control();
    screen_fader_.clear();
}

void Teleport::spawn() {
    spawn({0, 0});
}

void Teleport::despawn() {
    set_event_state(EventState::NotStarted);
}

void Teleport::update() {

    switch (event_state()) {
    case EventState::NotStarted:
        for ( size_t i = 0; i < teleporters_count_; ++i ) {
            auto * teleporter = &teleporters_[i];
            if ( teleporter->try_entry() ) {
                active_teleporter_ = teleporter;
                set_event_state(EventState::Playing);
                screen_fader_.request_fade_out();
                lock_control();
            }
        }
        break;

    case EventState::Playing:
        if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
            player_.set_position(active_teleporter_->destination());
            if ( camera_ ) {
                camera_->snap_to_target();
            }
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

void Teleport::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
    if ( event_state() == EventState::Playing ) {
        screen_fader_.update(fb);
    }
}

void Teleport::set_camera(CameraT *camera) {
    camera_ = camera;
}

}// namepsace event
}// namespace app


