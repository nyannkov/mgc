#include "attack_lance.hpp"
#include "resources/generated/anim/attack_lance/anim_attack_lance.h"
#include "resources/mml/mml.h"
#include "entity/player/player.hpp"
#include "entity/enemy/enemy.hpp"

namespace app {
namespace attack {


AttackLance::AttackLance(
    const FrameTimerT& frame_timer, 
    SoundControllerT& sound
) : anim_(frame_timer),
    sound_(sound) {

    this->set_position({0, 0});
    this->mut_sprite().set_visible(false);

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({8, 16});
    hitboxes[0].set_enabled(false);
}

void AttackLance::spawn(
    const mgc::math::Vec2i& pos,
    AttackOwner owner
) {
    spawn(pos, AttackLanceType::Hold, owner);
}

void AttackLance::spawn(
    const mgc::math::Vec2i& pos,
    AttackLanceType type,
    AttackOwner owner
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    hitboxes[0].set_size({16, 16});

    attack_type_ = type;

    switch (attack_type_) {
    case AttackLanceType::ThrowLeft:
        this->set_direction(AttackDirection::Left);
        break;
    default:
        this->set_direction(AttackDirection::Right);
        break;
    }
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    launch_state_ = LaunchState::Stop;
    switch (attack_type_) {
    case AttackLanceType::Hold:
        anim_type_ = AnimType::Up;
        break;
    case AttackLanceType::LeftwardAim:
        anim_type_ = AnimType::UpLeft;
        break;
    case AttackLanceType::RightwardAim:
        anim_type_ = AnimType::UpRight;
        break;
    case AttackLanceType::UpwardAim:
        anim_type_ = AnimType::Down;
        break;
    case AttackLanceType::ThrowLeft:
        anim_type_ = AnimType::Left;
        break;
    case AttackLanceType::ThrowRight:
        anim_type_ = AnimType::Right;
        break;
    case AttackLanceType::ThrowUpLeft:
        anim_type_ = AnimType::UpLeft;
        break;
    case AttackLanceType::ThrowUpRight:
        anim_type_ = AnimType::UpRight;
        break;
    case AttackLanceType::ThrowUp:
        anim_type_ = AnimType::Up;
        break;
    default:
        anim_type_ = AnimType::Up;
        break;
    }
    anim_.set_anim_frames(get_anim_frames(anim_type_));
    anim_.set_loop(true);

    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
}

void AttackLance::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void AttackLance::update_movement() {
    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    if ( attack_type_ == AttackLanceType::Hold  ||
         attack_type_ == AttackLanceType::LeftwardAim ||
         attack_type_ == AttackLanceType::RightwardAim ||
         attack_type_ == AttackLanceType::UpwardAim
    ) {
        if ( owner_ ) {
            this->set_position(owner_->position() + offset_);
        }
    } else {
        
        if ( launch_state_ == LaunchState::Start ) {
            switch (attack_type_) {
            case AttackLanceType::ThrowLeft:
                anim_type_ = AnimType::Left;
                velocity_.x = -8;
                velocity_.y = -1;
                this->set_position(owner_->position() + offset_);
                break;
            case AttackLanceType::ThrowRight:
                anim_type_ = AnimType::Right;
                velocity_.x = 8;
                velocity_.y = -1;
                this->set_position(owner_->position() + offset_);
                break;
            case AttackLanceType::ThrowUpLeft:
                anim_type_ = AnimType::UpLeft;
                velocity_.x = -8;
                velocity_.y = -8;
                this->set_position(owner_->position() + offset_);
                break;
            case AttackLanceType::ThrowUpRight:
                anim_type_ = AnimType::UpRight;
                velocity_.x = 8;
                velocity_.y = -8;
                this->set_position(owner_->position() + offset_);
                break;
            case AttackLanceType::ThrowUp:
                anim_type_ = AnimType::Up;
                velocity_.x = 0;
                velocity_.y = -10;
                this->set_position(owner_->position() + offset_);
                break;
            case AttackLanceType::Hold:
            case AttackLanceType::LeftwardAim:
            case AttackLanceType::RightwardAim:
            case AttackLanceType::UpwardAim:
            default:
                break;
            }
            launch_state_ = LaunchState::Launched;
        }

        auto pos = this->precise_position();
        pos.x += velocity_.x;
        pos.y += velocity_.y;
        if ( velocity_.y <= 12 ) {
            velocity_.y += 1;
        }
        this->set_precise_position(pos);
    }
}

void AttackLance::update_animation() {
    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

int32_t AttackLance::apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const {
    int32_t amount = 0;
    if ( attack_hitbox_index == 0 ) {
        if ( owner_type() == AttackOwner::Player ) {
            amount = 1;
            enemy.receive_damage(amount);
            switch (attack_type_) {
            case AttackLanceType::ThrowLeft:
                enemy.receive_impact({-12.0f, 0.0f});
                break;
            case AttackLanceType::ThrowRight:
                enemy.receive_impact({12.0f, 0.0f});
                break;
            default:
                break;
            }
        }
    }
    return amount;
}

int32_t AttackLance::apply_damage_to(Player& player, size_t attack_hitbox_index) const {
    int32_t amount = 0;
    if ( attack_hitbox_index == 0 ) {
        if ( owner_type() == AttackOwner::Enemy ) {
            amount = 1;
            player.receive_damage(amount);

            switch (attack_type_) {
            case AttackLanceType::ThrowLeft:
                player.receive_impact({-12.0f, 0.0f});
                break;
            case AttackLanceType::ThrowRight:
                player.receive_impact({12.0f, 0.0f});
                break;
            default:
                break;
            }
        }
    }

    return amount;
}

void AttackLance::on_enemy_hit(
    const enemy::Enemy& enemy,
    const mgc::collision::BoxCollisionInfo& info
) {
    switch (info.other_hitbox_index) {
    case static_cast<size_t>(enemy::EnemyHitboxIndex::Body): //fallthrough
    case static_cast<size_t>(enemy::EnemyHitboxIndex::Core):
        break;
    default:
        break;
    }
}

void AttackLance::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
}

void AttackLance::draw_wrap(
    FramebufferT& fb,
    mgc::math::Vec2i& cam_pos
) {
    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    this->draw(fb, cam_pos);
}

const mgc::parts::assets::AnimFrames& 
AttackLance::get_anim_frames(AnimType type) {
    switch (type) {
    case AnimType::Up: return anim_attack_lance_lance_up;
    case AnimType::Down: return anim_attack_lance_lance_down;
    case AnimType::Left: return anim_attack_lance_lance_left;
    case AnimType::UpLeft: return anim_attack_lance_lance_up_left;
    case AnimType::DownLeft: return anim_attack_lance_lance_down_left;
    case AnimType::Right: return anim_attack_lance_lance_right;
    case AnimType::UpRight: return anim_attack_lance_lance_up_right;
    case AnimType::DownRight: return anim_attack_lance_lance_down_right;
    default: return anim_attack_lance_lance_up;
    }
}
    

}// namespace attack
}// namespace app

