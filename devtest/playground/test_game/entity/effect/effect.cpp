#include "effect.hpp"
#include "resources/generated/tileset/tileset_effect.h"

namespace app {

Effect::Effect(const FrameTimerT& frame_timer, const GamepadT& gamepad)
    : frame_timer_(frame_timer),
      gamepad_(gamepad) { 

    this->set_position({0, 0});
    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_effect);

    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(8, 16);
    hitboxes[0].enabled = false;
}

void Effect::spawn(const mgc::math::Vec2i& pos, bool is_right) {

    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = true;
    is_right_ = is_right;

    this->set_position(pos);
    this->sprite().set_visible(true);
    
    if ( is_right ) {
        this->sprite().set_tile_index(0);
    } else {
        this->sprite().set_tile_index(1);
    }
}

void Effect::despawn() {

    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = false;
    this->sprite().set_visible(false);
}


}// namespace app
