#include "scratch.hpp"
#include "resources/generated/tileset/tileset_attack.h"
#include "entity/player/player.hpp"
#include "entity/enemy/enemy.hpp"


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

int32_t Scratch::apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const {
    int32_t amount = 0;
    if ( attack_hitbox_index == 0 ) {
        if ( owner_type() == AttackOwner::Player ) {
            amount = 1;
            enemy.receive_damage(amount);
            if ( this->direction() == AttackDirection::Right ) {
                enemy.receive_impact({12.0f, 0.0f});
            } else {
                enemy.receive_impact({-12.0f, 0.0f});
            }
        }
    }
    return amount;
}

int32_t Scratch::apply_damage_to(Player& player, size_t attack_hitbox_index) const {
    int32_t amount = 0;
    if ( attack_hitbox_index == 0 ) {
        if ( owner_type() == AttackOwner::Enemy ) {
            amount = 1;
            player.receive_damage(amount);
            if ( this->direction() == AttackDirection::Right ) {
                player.receive_impact({12.0f, 0.0f});
            } else {
                player.receive_impact({-12.0f, 0.0f});
            }
        }
    }

    return amount;
}

}// namespace app
