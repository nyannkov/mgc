#include "player.hpp"
#include "entity/enemy/enemy.hpp"
#include "entity/item/item.hpp"
#include "resources/mml/mml.h"

namespace app {

using mgc::platform::input::Key;

Player::Player(const FrameTimerT& frame_timer, const GamepadT& gamepad, SoundControllerT& sound_controller)
    : gamepad_(gamepad),
      frame_timer_(frame_timer),
      sound_controller_(sound_controller),
      attack_(frame_timer, gamepad),
      anim_(frame_timer), 
      velocity_({0.0f, 0.0f}),
      force_ex_({0.0f, 0.0f}),
      anim_state_(PlayerAnimState::StandRight),
      anim_state_manual_(PlayerAnimState::StandRight),
      anim_mode_(PlayerAnimMode::Auto),
      player_state_(PlayerState::Normal),
      is_right_(true),
      is_grounded_(true),
      is_invulnerable_(false),
      hit_ladder_(false),
      hit_one_way_block_(false),
      one_way_block_falling_(false),
      input_enabled_(true),
      gold_(100),
      attack_state_(AttackState::Stop),
      blink_animator_(frame_timer) {

      init();
}

void Player::init() {
    this->spawn({0, 0}, anim_state_);
    this->set_full_hp(6);
    this->set_hp(full_hp_);
    gold_ = 100;
    blink_animator_.set_target(*this);

    auto& body = at(this->hitboxes(), PlayerHitboxIndex::Body);
    body.offset = mgc::collision::HitboxOffset(1, 0);
    body.size = mgc::collision::HitboxSize(14, 16);
    body.enabled = true;
}

void Player::spawn(const mgc::math::Vec2i& pos, PlayerAnimState anim_state) {
    
    anim_state_ = anim_state;
    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();
    anim_.set_loop(true);
    anim_.set_current_frame(this->sprite());
    this->set_position(pos);

    is_grounded_ = true;
    velocity_ = {0.0f, 0.0f};

    player_state_ = PlayerState::Normal;
}

void Player::reset_state_for_placement(
    const mgc::math::Vec2i& pos, PlayerAnimState anim_state
) {
    anim_state_ = anim_state;
    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();
    anim_.set_loop(true);
    anim_.set_current_frame(this->sprite());
    this->set_position(pos);
    velocity_ = {0.0f, 0.0f};
    player_state_ = PlayerState::Normal;
}

void Player::update_movement() {

    auto real_pos = this->precise_position();

    if ( ( player_state_ == PlayerState::Ladder ) && is_grounded_ ) {
        player_state_ = PlayerState::Normal;
    }

    if ( player_state_ != PlayerState::GameOver ) {
        if ( hit_ladder_ ) {
            if ( ( gamepad_.is_pressed(Key::Up) ) ||
                 ( gamepad_.is_pressed(Key::Down) )
            ) {
                player_state_ = PlayerState::Ladder;
                one_way_block_falling_ = false;
            } else { 
            }
            hit_ladder_ = false;
        } else {
            player_state_ = PlayerState::Normal;
        }
    }

    // Update position
    if ( player_state_ == PlayerState::Normal ) {
        if ( input_enabled_ ) {
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
            }

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

    } else if ( player_state_ == PlayerState::Ladder ) {
        velocity_.y = 0;
        if ( input_enabled_ ) {
            if ( gamepad_.is_pressed(Key::Up) ) {
                real_pos.y -= 4;
            } else if ( gamepad_.is_pressed(Key::Down) ) {
                real_pos.y += 4;
            } else { }

            if ( gamepad_.is_pressed(Key::Left) ) {
                real_pos.x -= 4;
                is_right_ = false;
            } else if ( gamepad_.is_pressed(Key::Right) ) {
                real_pos.x += 4;
                is_right_ = true;
            } else { }
        }
        
    } else if ( player_state_ == PlayerState::GameOver ) {
        if ( velocity_.y < (MGC_CELL_LEN-1) ) {
            velocity_.y += 1.0f;
        }
        real_pos.y += velocity_.y;
    } else { }

    real_pos += force_ex_;

    this->set_precise_position(real_pos);

    if ( is_grounded_ ) {
        one_way_block_falling_ = false;
    }
    hit_one_way_block_ = false;

    is_grounded_ = false;

    force_ex_ *= 0.5;
}

void Player::update_animation(bool is_talking) {
    
    if ( anim_mode_ == PlayerAnimMode::Auto ) {
        if ( this->is_game_over() ) {
            update_anim_game_over();
        } else {
            if ( attack_state_ != AttackState::Stop ) {
                if ( !is_talking ) {
                    update_anim_attacking();
                } else {
                    attack_state_ = AttackState::Stop;
                    update_anim_normal();
                }
            } else {
                update_anim_normal();
            }
        }

        blink_animator_.update();

        if ( is_invulnerable_ ) {
            if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {
                blink_animator_.clear();
                is_invulnerable_ = false;
            }
        }
    } else if ( anim_mode_ == PlayerAnimMode::Manual ) {
        anim_.proc();
        anim_.set_current_frame(this->sprite());
    }
}

void Player::receive_damage(int32_t amount) {

    int32_t next_hp = hp() - amount;

    set_hp( next_hp > 0 ? next_hp : 0 );
}

void Player::receive_heal(int32_t amount) {

    int32_t next_hp = hp() + amount;

    set_hp( next_hp < full_hp() ? next_hp : full_hp() );
}

void Player::receive_life_up(int32_t amount) {

    int32_t next_full_hp = full_hp() + amount;

    set_full_hp( next_full_hp < 20 ? next_full_hp : 20 );
}

void Player::receive_impact(mgc::math::Vec2f delta) {
    
    force_ex_ += delta;
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
    } else if ( player_state_ == PlayerState::Ladder ) {
        if ( gamepad_.is_pressed(Key::Up) || gamepad_.is_pressed(Key::Down) ) {
            state_next = PlayerAnimState::Ladder;
        } else {
            state_next = PlayerAnimState::LadderStay;
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

    if ( attack_state_ == AttackState::Start ) {
        attack_state_ = AttackState::InProgress;
        sound_controller_.play_sound_effect(MML_SE_4_ATTACK_SCRATCH, 0.0);

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

void Player::set_anim_manually(PlayerAnimState state) {
    attack_state_ = AttackState::Stop;
    attack_.despawn();
    anim_state_manual_ = state;
    anim_.set_anim_frames(get_anim_frames(state));
    anim_.set_loop(true);
    anim_.start_animation();
}

void Player::update_anim_game_over() {

    anim_.set_loop(true);

    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();

    anim_.proc();
    anim_.set_current_frame(this->sprite());
}

void Player::set_game_over() { 
    player_state_ = PlayerState::GameOver;
    if ( is_right_ ) {
        anim_state_ = PlayerAnimState::GameOverRight;
    } else {
        anim_state_ = PlayerAnimState::GameOverLeft;
    }
}

void Player::revive() {
    this->player_state_ = PlayerState::Normal;
    this->set_hp(full_hp_);
}

void Player::on_enemy_hit(
    const enemy::Enemy& enemy,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( !is_invulnerable_ && !this->is_game_over() ) {
        size_t enemy_hitbox_index = info.other_hitbox_index;
        if ( enemy.apply_damage_to(*this, enemy_hitbox_index) > 0 ) {
            if ( this->hp() > 0 ) {
                sound_controller_.play_sound_effect(MML_SE_3_DAMAGE, 0.0);
                is_invulnerable_ = true;
                blink_animator_.set_blink_half_period(50);
                blink_animator_.set_blink_count_max(40);
                blink_animator_.set_end_state(mgc::utils::BlinkEndState::Visible);
                blink_animator_.start();
            }
        }
    }
}

void Player::on_item_hit(
    const item::Item& item,
    const mgc::collision::BoxCollisionInfo& info
) {
    size_t item_hitbox_index = info.other_hitbox_index;
    if ( item.apply_effect_to(*this, item_hitbox_index) > 0 ) {
        if ( item.effect_type() == item::ItemEffectType::Heal ) {
            sound_controller_.play_sound_effect(MML_SE_1_LIFE_RECOVER, 0.0);

        } else if ( item.effect_type() == item::ItemEffectType::LifeUp ) {
            sound_controller_.play_sound_effect(MML_SE_2_LIFE_UP, 0.0);
        } else { }
    }
}


void Player::on_collision_resolved(
    const stage::LayerBlock& block,
    const mgc::collision::MapPushbackInfo& info
) {
    auto pos = this->position();
    // Pushback response logic: stop falling or bounce depending on direction
    if ( info.pushback.y < 0 ) {
        velocity_.y = 0.0f;
        is_grounded_ = true;
    } else if ( info.pushback.y > 0 ) {
        velocity_.y *= -1;
    } else { 
    }
    pos += info.pushback;

    this->set_position(pos);
}

void Player::on_collision_resolved(
    const stage::LayerOneWayBlock& block,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( velocity_.y < 0 ) {
        return;
    }

    if ( gamepad_.hold_counter(Key::Down) >= 5 ) {
        one_way_block_falling_ = true;
        return;
    }

    if ( one_way_block_falling_ ) {
        return;
    }

    if ( player_state_ == PlayerState::Ladder ) {
        return;
    }

    auto pos = this->position();
    if ( info.pushback.y < 0 ) {
        velocity_.y = 0.0f;
        is_grounded_ = true;
    } else if ( info.pushback.y > 0 ) {
        velocity_.y *= -1;
    } else { 
    }
    pos.y += info.pushback.y;

    this->set_position(pos);
}

}// namespace app

