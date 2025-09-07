#include "player.hpp"

namespace app {

using mgc::platform::input::Key;

Player::Player(const FrameTimerT& frame_timer, const GamepadT& gamepad)
    : gamepad_(gamepad),
      frame_timer_(frame_timer),
      attack_(frame_timer, gamepad),
      anim_(frame_timer), 
      velocity_({0.0f, 0.0f}),
      anim_state_(PlayerAnimState::StandRight),
      is_right_(true),
      is_grounded_(true),
      is_grounded_pre_(true),
      attack_state_(AttackState::Stop),
      hp_(MAX_PLAYER_HP),
      blink_animator_(frame_timer) {

    this->set_spawn_point({0, 0}, anim_state_);

    auto& body = at(this->hitboxes(), PlayerHitboxIndex::Body);
    body.offset = mgc::collision::HitboxOffset(0, 0);
    body.size = mgc::collision::HitboxSize(16, 16);
    body.enabled = true;
}

void Player::set_spawn_point(const mgc::math::Vec2i& pos, PlayerAnimState anim_state) {
    
    anim_state_ = anim_state;
    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();
    anim_.set_loop(true);
    anim_.set_current_frame(this->sprite());
    this->set_position(pos);

    is_grounded_ = true;
    velocity_ = {0.0f, 0.0f};
}

void Player::prepare_update() {

    auto real_pos = this->precise_position();

    // Update position
    if ( attack_state_ == AttackState::Stop ) {
        if ( gamepad_.just_pressed(Key::Enter) ) {
            attack_state_ = AttackState::Start;
        } else if ( gamepad_.is_pressed(Key::Left) ) {
            real_pos.x -= 4;
            is_right_ = false;
        } else if ( gamepad_.is_pressed(Key::Right) ) {
            real_pos.x += 4;
            is_right_ = true;
        } else { }

        if ( gamepad_.just_pressed(Key::Cancel) ) {
            if ( is_grounded_ ) {
                velocity_.y = -12;
            }
        }
    }

    if ( velocity_.y < (MGC_CELL_LEN-1) ) {
        velocity_.y += 1.0f;
    }
    real_pos.y += velocity_.y;

    this->set_precise_position(real_pos);

    is_grounded_ = false;
}

void Player::finalize_update() {

    // Update animation
    if ( attack_state_ != AttackState::Stop ) {
        update_anim_attacking();
    } else {
        update_anim_normal();
    }
}

void Player::update_anim_normal() {

    PlayerAnimState state_next = anim_state_;
    anim_.set_loop(true);

    if ( is_grounded_ ) {
        if ( gamepad_.is_pressed(Key::Left) || gamepad_.is_pressed(Key::Right) ) {
            state_next = is_right_ ? PlayerAnimState::WalkRight
                                   : PlayerAnimState::WalkLeft;
        } else {
            state_next = is_right_ ? PlayerAnimState::StandRight
                                   : PlayerAnimState::StandLeft;
        }
    } else {
        state_next = is_right_ ? PlayerAnimState::JumpRight
                               : PlayerAnimState::JumpLeft;
    }

    if ( state_next != anim_state_ ) {
        anim_state_ = state_next;
        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();
    }

    anim_.proc();
    anim_.set_current_frame(this->sprite());
}

void Player::update_anim_attacking() {

    anim_.set_loop(false);
    attack_.set_damage(1);

    if ( attack_state_ == AttackState::Start ) {
        attack_state_ = AttackState::InProgress;

        if ( is_right_ ) {
            attack_.spawn(this->position() + mgc::math::Vec2i(18, 0), AttackOwner::Player, AttackDirection::Right);
            anim_state_ = PlayerAnimState::AttackRight;
        } else {
            attack_.spawn(this->position() + mgc::math::Vec2i(-10, 0), AttackOwner::Player, AttackDirection::Left);
            anim_state_ = PlayerAnimState::AttackLeft;
        }

        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();

    } else if ( attack_state_ == AttackState::InProgress ) {
        if ( anim_.is_finished() ) {
            attack_state_ = AttackState::Stop;
            attack_.despawn();
        }

    } else { }

    anim_.proc();
    anim_.set_current_frame(this->sprite());
}


}// namespace app

