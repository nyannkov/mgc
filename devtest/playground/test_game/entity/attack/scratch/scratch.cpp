#include "scratch.hpp"
#include "resources/generated/tileset/tileset_attack.h"

namespace app {

Scratch::Scratch(const FrameTimerT& frame_timer, const GamepadT& gamepad)
    : frame_timer_(frame_timer),
      gamepad_(gamepad) { 

    this->set_position({0, 0});
    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_attack);

    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(8, 16);
    hitboxes[0].enabled = false;

    this->set_damage(1);
}

void Scratch::spawn(const mgc::math::Vec2i& pos, AttackOwner owner, AttackDirection dir) { 

    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = true;
    this->set_direction(dir);
    this->set_position(pos);
    this->sprite().set_visible(true);
    
    if ( this->direction() == AttackDirection::Right ) {
        this->sprite().set_tile_index(0);
    } else {
        this->sprite().set_tile_index(1);
    }
}

void Scratch::despawn() {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = false;
    this->sprite().set_visible(false);
}

}// namespace app
