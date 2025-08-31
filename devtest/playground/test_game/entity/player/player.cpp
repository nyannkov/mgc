#include "player.hpp"

namespace app {

using mgc::platform::input::Key;

Player::Player(const FrameTimerT& frame_timer, const GamepadT& gamepad)
    : gamepad_(gamepad),
      frame_timer_(frame_timer),
      anim_(frame_timer), 
      velocity_({0.0f, 0.0f}),
      anim_state_(PlayerAnimState::StandRight),
      is_right_(true),
      is_grounded_(true),
      is_grounded_pre_(true) {

    this->set_spawn_point({0, 0}, anim_state_);

    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(16, 16);
    hitboxes[0].enabled = true;
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

    auto real_pos = this->internal_position();

    // Update position
    if ( gamepad_.is_pressed(Key::Left) ) {
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

    if ( velocity_.y < (MGC_CELL_LEN-1) ) {
        velocity_.y += 1.0f;
    }
    real_pos.y += velocity_.y;

    this->set_internal_position(real_pos);
    this->commit_position();

    is_grounded_ = false;
}

void Player::finalize_update() {

    // Update animation
    PlayerAnimState state_next = anim_state_;

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

}// namespace app

