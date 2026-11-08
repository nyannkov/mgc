#include "light_effect.hpp"

namespace app {
namespace event {

using app::event::EventState;

LightEffect::LightEffect(
        const mgc::features::HasPosition<mgc::math::Vec2i>& player_pos
    ) : player_pos_(player_pos) {
    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
    wide_mode_ = false;
    offset_ = { 8, 8 };
    light_.set_raduis(20);
}

void LightEffect::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    set_wide_mode(false);
}

void LightEffect::spawn() {
    spawn({0, 0});
}

void LightEffect::despawn() {
    set_event_state(EventState::NotStarted);
}

void LightEffect::update() {
    light_.set_position(player_pos_.position() + offset_);
}

void LightEffect::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
    light_.draw(fb, cam_pos);
}

void LightEffect::set_wide_mode(bool m) {
    wide_mode_ = m;
    if ( wide_mode_ ) {
        light_.set_raduis(60);
    } else {
        light_.set_raduis(20);
    }
}

} // namespace event
} // namespace app

