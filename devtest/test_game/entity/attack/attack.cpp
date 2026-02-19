#include "attack.hpp"
#include "resources/generated/anim/attack/anim_attack.h"
#include "resources/mml/mml.h"
#include "entity/player/player.hpp"
#include "entity/enemy/enemy.hpp"

namespace app {

Attack::Attack(
    const FrameTimerT& frame_timer, 
    const GamepadT& gamepad,
    SoundControllerT& sound
) : anim_(frame_timer),
    gamepad_(gamepad),
    sound_(sound) { 

    this->set_position({0, 0});
    this->mut_sprite().set_visible(false);

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({8, 16});
    hitboxes[0].set_enabled(false);

    sound_.play_sound_effect(MML_SE_4_ATTACK_SCRATCH, 0.0);
}

void Attack::spawn(
    const mgc::math::Vec2i& pos,
    AttackType type,
    AttackOwner owner,
    AttackDirection dir
) {
    set_attack_type(type);

    switch ( attack_type() ) {
    case AttackType::Scratch:
        spawn_scratch(pos, owner, dir);
        break;
    case AttackType::Boomerang:
        spawn_boomerang(pos, owner, dir);
        break;
    default:
        break;
    }

    pos_orig_ = this->position();
}

void Attack::despawn() {
    switch ( attack_type() ) {
    case AttackType::Scratch:
        despawn_scratch();
        break;
    case AttackType::Boomerang:
        despawn_boomerang();
        break;
    default:
        break;
    }
}

void Attack::update_movement() {
    switch ( attack_type() ) {
    case AttackType::Scratch:
        update_movement_scratch();
        break;
    case AttackType::Boomerang:
        update_movement_boomerang();
        break;
    default:
        break;
    }
}

void Attack::update_animation() {
    switch ( attack_type() ) {
    case AttackType::Scratch:
        update_animation_scratch();
        break;
    case AttackType::Boomerang:
        update_animation_boomerang();
        break;
    default:
        break;
    }
}

int32_t Attack::apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const {
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

int32_t Attack::apply_damage_to(Player& player, size_t attack_hitbox_index) const {
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

void Attack::spawn_scratch(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    if ( this->direction() == AttackDirection::Right ) {
        hitboxes[0].set_offset({0, 0});
        anim_.set_anim_frames(anim_attack_scratch_right);
        anim_.set_loop(false);
    } else {
        hitboxes[0].set_offset({8, 0});
        anim_.set_anim_frames(anim_attack_scratch_left);
        anim_.set_loop(false);
    }
    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
}

void Attack::despawn_scratch() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void Attack::update_animation_scratch() {
    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    if ( anim_.is_finished() ) {
        despawn();
    } else {
        anim_.proc();
        anim_.set_current_frame(this->mut_sprite());
    }
}

void Attack::update_movement_scratch() {
}


void Attack::spawn_boomerang(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    if ( this->direction() == AttackDirection::Right ) {
        hitboxes[0].set_offset({0, 0});
        anim_.set_anim_frames(anim_attack_boomerang_right);
        anim_.set_loop(true);
        set_velocity({12, 0});
    } else {
        hitboxes[0].set_offset({0, 0});
        anim_.set_anim_frames(anim_attack_boomerang_left);
        anim_.set_loop(true);
        set_velocity({-12, 0});
    }
    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
}

void Attack::despawn_boomerang() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void Attack::update_animation_boomerang() {
    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

void Attack::update_movement_boomerang() {

    if ( anim_.is_finished() ) {
        despawn();
        return;
    }

    if ( MGC_ABS(pos_orig_.x - this->position().x) > 1000 ) {// TODO
        despawn();
        return;
    }

    auto pos = precise_position();
    auto v = velocity();
    if ( direction() == AttackDirection::Left ) {
        if ( v.x < 12 ) {
            v.x += 1;
        }
    } else {
        if ( v.x > -12 ) {
            v.x -= 1;
        }
    }

    pos.x += v.x;

    set_velocity(v);
    set_precise_position(pos);
}

}// namespace app
