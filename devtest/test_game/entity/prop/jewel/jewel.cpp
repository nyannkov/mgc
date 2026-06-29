#include "jewel.hpp"
#include "resources/generated/anim/prop/jewel/anim_jewel.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Jewel::Jewel(const GamepadT& gamepad, const FrameTimerT& frame_timer) 
    : gamepad_(gamepad),
      anim_(frame_timer) {

    anim_.set_anim_frames(anim_jewel_blinking);
    anim_.set_current_frame(this->mut_sprite());

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({8, 8});
    hitboxes[0].set_enabled(false);
}

void Jewel::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    anim_.set_loop(true);
    anim_.start_animation();
    found_ = false;
}

void Jewel::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    found_ = false;
}

void Jewel::update_animation() {
    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

void Jewel::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Down) ) {
        found_ = true;
    }
}

} // namespace prop
} // namespace app

