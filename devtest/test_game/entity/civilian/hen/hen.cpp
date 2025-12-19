#include "hen.hpp"
#include "resources/generated/tileset/tileset_hen.h"

namespace app {
namespace civilian {

using mgc::platform::input::Key;

Hen::Hen(const GamepadT& gamepad) 
    : gamepad_(gamepad) {

    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_hen);
    auto& hitboxes = this->hitboxes();
    at(hitboxes, CivilHitboxId::Body).offset = mgc::collision::HitboxOffset(0, 0);
    at(hitboxes, CivilHitboxId::Body).size = mgc::collision::HitboxSize(6, 10);
    at(hitboxes, CivilHitboxId::Body).enabled = false;

    at(hitboxes, CivilHitboxId::View).offset = mgc::collision::HitboxOffset(-16, 0);
    at(hitboxes, CivilHitboxId::View).size = mgc::collision::HitboxSize(38, 10);
    at(hitboxes, CivilHitboxId::View).enabled = false;
}

void Hen::spawn(const mgc::math::Vec2i& pos) {
    this->set_position(pos);
    auto& hitboxes = this->hitboxes();
    at(hitboxes, CivilHitboxId::Body).enabled = true;
    at(hitboxes, CivilHitboxId::View).enabled = true;
    this->sprite().set_visible(true);
}

void Hen::spawn(const mgc::math::Vec2i& pos, HenAnimState anim_state) {
    this->set_anim_state(anim_state);
    this->spawn(pos);
}

void Hen::despawn() {
    auto& hitboxes = this->hitboxes();
    at(hitboxes, CivilHitboxId::Body).enabled = false;
    at(hitboxes, CivilHitboxId::View).enabled = false;
    this->sprite().set_visible(false);
}

void Hen::update_movement() {
}

void Hen::update_animation() {
}

void Hen::set_anim_state(HenAnimState anim_state) {
    switch (anim_state) {
    case HenAnimState::Stand_Left:
        this->sprite().set_tile_index(1);
        break;
    case HenAnimState::Stand_Right:
        this->sprite().set_tile_index(3);
        break;
//    case HenAnimState::Crouch_Left:
//        break;
//    case HenAnimState::Crouch_Right:
//        break;
//    case HenAnimState::LookUp_Left:
//        break;
    case HenAnimState::LookUp_Right:
        this->sprite().set_tile_index(4);
        break;
    default:
        break;
    }
    anim_state_ = anim_state;
}

void Hen::on_player_in_view(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Enter) ) {
        trigger_talkflow_request();
    }
}

}// namespace enemy
}// namespace app

