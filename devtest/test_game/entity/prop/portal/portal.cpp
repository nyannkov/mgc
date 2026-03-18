#include "portal.hpp"
#include "resources/generated/anim/prop/portal/anim_portal.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Portal::Portal(const GamepadT& gamepad, const FrameTimerT& frame_timer) 
    : gamepad_(gamepad),
      anim_(frame_timer) {

    anim_.set_anim_frames(anim_portal_blinking);
    anim_.set_current_frame(this->mut_sprite());

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({1, 0});
    hitboxes[0].set_size({16, 24});
    hitboxes[0].set_enabled(false);
}

void Portal::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    anim_.set_loop(true);
    anim_.start_animation();
}

void Portal::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Portal::update_animation() {
    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

void Portal::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Up) ) {
        try_entry_ = true;
    }
}

} // namespace prop
} // namespace app

