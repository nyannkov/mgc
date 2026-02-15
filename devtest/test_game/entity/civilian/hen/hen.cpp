#include "hen.hpp"
#include "resources/generated/tileset/tileset_hen.h"

namespace app {
namespace civilian {

using mgc::platform::input::Key;

Hen::Hen(const GamepadT& gamepad) 
    : gamepad_(gamepad) {

    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_hen);
    auto& hitboxes = this->mut_hitboxes();
    at(hitboxes, CivilHitboxId::Body).set_offset({0, 0});
    at(hitboxes, CivilHitboxId::Body).set_size({6, 10});
    at(hitboxes, CivilHitboxId::Body).set_enabled(false);

    at(hitboxes, CivilHitboxId::View).set_offset({-16, 0});
    at(hitboxes, CivilHitboxId::View).set_size({38, 10});
    at(hitboxes, CivilHitboxId::View).set_enabled(false);
}

void Hen::spawn(const mgc::math::Vec2i& pos) {
    this->set_position(pos);
    auto& hitboxes = this->mut_hitboxes();
    at(hitboxes, CivilHitboxId::Body).set_enabled(true);
    at(hitboxes, CivilHitboxId::View).set_enabled(true);
    this->mut_sprite().set_visible(true);
}

void Hen::spawn(const mgc::math::Vec2i& pos, HenAnimState anim_state) {
    this->set_anim_state(anim_state);
    this->spawn(pos);
}

void Hen::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    at(hitboxes, CivilHitboxId::Body).set_enabled(false);
    at(hitboxes, CivilHitboxId::View).set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Hen::update_movement() {
}

void Hen::update_animation() {
}

void Hen::set_anim_state(HenAnimState anim_state) {
    switch (anim_state) {
    case HenAnimState::Stand_Left:
        this->mut_sprite().set_tile_index(1);
        break;
    case HenAnimState::Stand_Right:
        this->mut_sprite().set_tile_index(3);
        break;
//    case HenAnimState::Crouch_Left:
//        break;
//    case HenAnimState::Crouch_Right:
//        break;
//    case HenAnimState::LookUp_Left:
//        break;
    case HenAnimState::LookUp_Right:
        this->mut_sprite().set_tile_index(4);
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

