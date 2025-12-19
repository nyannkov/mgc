#include "chick.hpp"
#include "resources/generated/tileset/tileset_chick.h"

namespace app {
namespace civilian {

using mgc::platform::input::Key;

Chick::Chick(const GamepadT& gamepad) 
    : gamepad_(gamepad) {
    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_chick);
    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(6, 10);
    hitboxes[0].enabled = false;
}

void Chick::spawn(const mgc::math::Vec2i& pos) {
    this->set_position(pos);
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = true;
    this->sprite().set_visible(true);
}

void Chick::spawn(const mgc::math::Vec2i& pos, ChickAnimState anim_state) {
    this->set_anim_state(anim_state);
    this->spawn(pos);
}

void Chick::despawn() {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = false;
    this->sprite().set_visible(false);
}

void Chick::update_movement() {
}

void Chick::update_animation() {
}

void Chick::set_anim_state(ChickAnimState anim_state) {
    switch (anim_state) {
    case ChickAnimState::Stand_Left:
        this->sprite().set_tile_index(1);
        break;
    case ChickAnimState::Stand_Right:
        this->sprite().set_tile_index(3);
        break;
//    case ChickAnimState::Crouch_Left:
//        break;
//    case ChickAnimState::Crouch_Right:
//        break;
//    case ChickAnimState::LookUp_Left:
//        break;
    case ChickAnimState::LookUp_Right:
        this->sprite().set_tile_index(4);
        break;
    default:
        break;
    }
    anim_state_ = anim_state;
}

}// namespace enemy
}// namespace app

