#include "lancer.hpp"
#include "lancer_anim.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

Lancer::Lancer(
    const FrameTimerT& timer,
    SoundControllerT& sound,
    const mgc::features::HasPosition<mgc::math::Vec2i>& target_pos
) : 
    target_pos_(target_pos),
    sound_(sound),
    anim_(timer), 
    sw_(timer),
    force_ex_({0.0f, 0.0f}),
    blink_animator_(timer),
    lance_{
        attack::AttackLance(timer, sound), 
        attack::AttackLance(timer, sound), 
        attack::AttackLance(timer, sound) 
    },
    weapons_{
        &lance_[0],
        &lance_[1],
        &lance_[2]
    } {
      blink_animator_.set_target(*this);
      blink_animator_.set_blink_half_period(50);
      blink_animator_.set_blink_count_max(40);
      set_hp(LancerMaxHP);
      set_full_hp(LancerMaxHP);
      set_money(10);
      
      for ( auto& lance : lance_ ) {
          lance.set_target(&target_pos);
          lance.set_owner(this);
      }
}

void Lancer::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    
    (void)is_right;

    this->set_enemy_state(EnemyState::Spawning);

    anim_state_ = LancerAnimState::WalkLeft;
    prev_anim_state_ = LancerAnimState::WalkLeft;
    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();
    anim_.set_loop(false);
    anim_.set_current_frame(this->mut_sprite());
    this->set_position(pos);

    auto& hitboxes = this->mut_hitboxes();

    // body
    auto& hitbox_body = at(hitboxes, EnemyHitboxIndex::Body);
    hitbox_body.set_offset({0, 0});
    hitbox_body.set_size({16, 32});
    hitbox_body.set_enabled(true);

    // TODO core
    auto& hitbox_core = at(hitboxes, EnemyHitboxIndex::ViewLeft);//TODO rename
    hitbox_core.set_offset({6, 22});
    hitbox_core.set_size({8, 10});
    hitbox_core.set_enabled(true);

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);

    spawn_lance(pos, lance_[0], attack::AttackLanceType::Hold);

    action_ = ActionState::Wait;
    fight_state_ = FightState::Wait;

    sw_.restart();
}

void Lancer::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    action_ = ActionState::Wait;
    fight_state_ = FightState::Wait;
    anim_state_ = LancerAnimState::SeeYouNext;
    this->set_enemy_state(EnemyState::Despawning);
    blink_animator_.set_end_state(mgc::utils::BlinkEndState::Hidden);

    blink_animator_.set_blink_half_period(50);
    blink_animator_.set_blink_count_max(40);
    blink_animator_.start();
    for ( auto& lance : lance_ ) {
        lance.despawn();
    }
}

void Lancer::update_movement() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {

        auto dx = target_pos_.position().x - this->position().x;
        auto dy = target_pos_.position().y - this->position().y;

        const int16_t DX_THRES = 16*4;
        const int16_t DY_THRES = 16*1;

        if ( fight_state_ == FightState::Wait ) {
            action_ = ActionState::Wait;
        } else if ( fight_state_ == FightState::Victory ) {
            action_ = ActionState::Victory;
        } else { }

        switch ( action_ ) {
        case ActionState::Wait:
            if ( fight_state_ == FightState::Fight ) {
                action_ = ActionState::Walking;
            }
            break;
        case ActionState::Victory:
            if ( anim_state_ != LancerAnimState::SeeYouNext ) {
                for ( auto& lance : lance_ ) {
                    lance.despawn();
                }
            }
            break;
        case ActionState::Walking:
            if ( MGC_ABS(dx) < DX_THRES ) {
                action_ = ActionState::ReadyToThrow;
            }
            break;
        case ActionState::ReadyToThrow:
            if ( anim_state_ == LancerAnimState::ThrowLeft &&
                 anim_.current_frame_index() == ANIM_BOSS_LANCER_THROW_LEFT_THROW_LEFT 
            ) {
                action_ = ActionState::Launch;
            } else if ( anim_state_ == LancerAnimState::ThrowRight &&
                 anim_.current_frame_index() == ANIM_BOSS_LANCER_THROW_RIGHT_THROW_RIGHT 
            ) {
                action_ = ActionState::Launch;
            } else if ( anim_state_ == LancerAnimState::ThrowUp &&
                 anim_.current_frame_index() == ANIM_BOSS_LANCER_THROW_UP_THROW_UP 
            ) {
                action_ = ActionState::Launch;
            }
            break;
        case ActionState::Launch:
            action_ = ActionState::Throwing;
            break;
        case ActionState::Throwing:
            
            if ( anim_.is_finished() ) {
                action_ = ActionState::Thrown;
            }
            break;
        case ActionState::Thrown:
            action_ = ActionState::Walking;
            spawn_lance(this->position(), lance_[0], attack::AttackLanceType::Hold);
            break;

        default:
            action_ = ActionState::Walking;
            break;
        }

        switch ( action_ ) {
        case ActionState::Wait:
            anim_state_ = LancerAnimState::Stand;
            break;
        case ActionState::Victory:
            anim_state_ = LancerAnimState::SeeYouNext;
            break;
        case ActionState::Walking: {
                auto pos = this->precise_position();
                uint32_t count = sound_.update_bgm_param_count();
                uint32_t param = sound_.last_bgm_param();
                if ( update_bgm_param_count_ != count ) {
                    update_bgm_param_count_ = count;
                    if ( rand()%2 == 0 ) {
                        velocity_.x = -8;
                        anim_state_ = LancerAnimState::WalkLeft;
                    } else {
                        velocity_.x = 8;
                        anim_state_ = LancerAnimState::WalkRight;
                    }
                }
            }
            break;
        case ActionState::ReadyToThrow: {
            attack::AttackLanceType attack_type;
            if ( dx < -8 ) {
                anim_state_ = LancerAnimState::ThrowLeft;
                attack_type = attack::AttackLanceType::LeftwardAim;
            } else if ( 8 < dx ) {
                anim_state_ = LancerAnimState::ThrowRight;
                attack_type = attack::AttackLanceType::RightwardAim;
            } else {
                anim_state_ = LancerAnimState::ThrowUp;
                attack_type = attack::AttackLanceType::UpwardAim;
            }
            spawn_lance(this->position(), lance_[0], attack_type);
            break;
        }
        case ActionState::Launch: {
            attack::AttackLanceType attack_type;
            if ( anim_state_ == LancerAnimState::ThrowUp ) {
                attack_type = attack::AttackLanceType::ThrowUp;
            } else {
                if ( dy > DY_THRES ) {
                    if ( anim_state_ == LancerAnimState::ThrowLeft ) {
                        attack_type = attack::AttackLanceType::ThrowLeft;
                    } else {
                        attack_type = attack::AttackLanceType::ThrowRight;
                    }
                } else {
                    if ( anim_state_ == LancerAnimState::ThrowLeft ) {
                        attack_type = attack::AttackLanceType::ThrowUpLeft;
                    } else {
                        attack_type = attack::AttackLanceType::ThrowUpRight;
                    }
                }
            }
            spawn_lance(this->position(), lance_[0], attack_type);
            break;
        }
        case ActionState::Throwing:
            break;
        case ActionState::Thrown:
            break;
        default:
            break;
        }

        auto pos = this->precise_position();
        pos += velocity_;
        velocity_.x *= 0.4;
        this->set_precise_position(pos);

        lance_[0].update_movement();
        blink_animator_.update();
        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {
            blink_animator_.clear();
        }
    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        auto pos = this->precise_position();
        velocity_.y -= 0.3;
        pos.y += velocity_.y;
        this->set_precise_position(pos);

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }
    } else { }
}

void Lancer::update_animation() {

    if ( this->enemy_state() == EnemyState::Inactive ) return;

    lance_[0].update_animation();

    if ( prev_anim_state_ != anim_state_ ||
         anim_.is_finished()
    ) {
        prev_anim_state_ = anim_state_;
        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();
    }

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

int32_t Lancer::apply_damage_to(
    Player& player,
    size_t enemy_hitbox_index
) const {
    int32_t amount = 0;
    if ( enemy_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body) ) {
        amount = 1;
        player.receive_damage(amount);
    }
    return amount;
}

void Lancer::receive_damage(int32_t amount) {
    blink_animator_.set_blink_half_period(100);
    blink_animator_.set_blink_count_max(5);
    blink_animator_.set_end_state(mgc::utils::BlinkEndState::Visible);
    blink_animator_.start();
    if ( this->hp() > amount ) {
        this->set_hp(hp() - amount);
    } else {
        this->set_hp(0);
    }
}

void Lancer::receive_impact(mgc::math::Vec2f delta, float dumping_rate) {
    force_ex_ += delta;
}

void Lancer::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) { 
}

void Lancer::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::ViewLeft)) { //TODO rename
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);
    }
}

void Lancer::on_collision_resolved(
    const stage::LayerBlock& layer,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( info.obj_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {

        if ( info.pushback.y < 0 ) {
            if ( velocity_.y > 0 ) {
                velocity_.y = 0.0f;
            }
        }
        
        auto pos = this->position();
        pos += info.pushback;

        this->set_position(pos);

        lance_[0].update_movement();
    }
}

void Lancer::spawn_lance(
        const mgc::math::Vec2i& pos,
        attack::AttackLance& lance,
        attack::AttackLanceType type
) {
    lance.spawn(
        pos,
        type,
        attack::AttackOwner::Enemy
    );

    switch ( type ) {
    case attack::AttackLanceType::Hold:
        lance.set_offset({8, 1});
        break;
    case attack::AttackLanceType::LeftwardAim:
        lance.set_offset({10, 1});
        break;
    case attack::AttackLanceType::RightwardAim:
        lance.set_offset({-6, 1});
        break;
    case attack::AttackLanceType::UpwardAim:
        lance.set_offset({8, 1});
        break;
    case attack::AttackLanceType::ThrowLeft:
        lance.set_offset({-8, 1});
        lance.launch();
        break;
    case attack::AttackLanceType::ThrowRight:
        lance.set_offset({8, 1});
        lance.launch();
        break;
    case attack::AttackLanceType::ThrowUpLeft:
        lance.set_offset({-8, 1});
        lance.launch();
        break;
    case attack::AttackLanceType::ThrowUpRight:
        lance.set_offset({8, 1});
        lance.launch();
        break;
    case attack::AttackLanceType::ThrowUp:
        lance.set_offset({8, -1});
        lance.launch();
    default:
        break;
    }
}
void Lancer::set_enabled_hitbox_body(bool enabled) {
    // body
    auto& hitboxes = this->mut_hitboxes();
    auto& hitbox_body = at(hitboxes, EnemyHitboxIndex::Body);
    hitbox_body.set_enabled(enabled);
}

}// namespace enemy
}// namespace app

