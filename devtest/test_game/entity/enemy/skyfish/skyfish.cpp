#include "skyfish.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

SkyFish::SkyFish(
    const FrameTimerT& timer,
    SoundControllerT& sound,
    const Player& player
) : player_(player),
    sound_(sound),
    anim_(timer), 
    bt_(timer),
    bt_listener_(timer),
    velocity_({0.0f, 0.0f}),
    force_ex_({0.0f, 0.0f}),
    anim_state_(SkyFishAnimState::HoverRight),
    blink_animator_(timer) {

      blink_animator_.set_target(*this);
      set_hp(SkyFishMaxHP);
      set_full_hp(SkyFishMaxHP);
      set_gold(10);
}

void SkyFish::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    
    this->set_enemy_state(EnemyState::Spawning);

    if ( is_right ) {
        anim_state_ = SkyFishAnimState::HoverRight;
    } else {
        anim_state_ = SkyFishAnimState::HoverLeft;
    }
    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();
    anim_.set_loop(true);
    anim_.set_current_frame(this->mut_sprite());
    this->set_position(pos);

    velocity_ = {0.0f, 0.0f};

    auto& hitboxes = this->mut_hitboxes();

    // body
    auto& hitbox_body = at(hitboxes, EnemyHitboxIndex::Body);
    hitbox_body.set_offset({0, 0});
    hitbox_body.set_size({16, 16});
    hitbox_body.set_enabled(true);

    // view left
    auto& hitbox_view_left = at(hitboxes, EnemyHitboxIndex::ViewLeft);
    hitbox_view_left.set_offset({8-16*7, 8-16*5});
    hitbox_view_left.set_size({16*7, 16*10});
    hitbox_view_left.set_enabled(true);

    // view right
    auto& hitbox_view_right = at(hitboxes, EnemyHitboxIndex::ViewRight);
    hitbox_view_right.set_offset({8, 8-16*5});
    hitbox_view_right.set_size({16*7, 16*10});
    hitbox_view_right.set_enabled(false);

    // Behavior Tree
    bt_listener_.clear_all_hit_flags();
    bt_.set_btree(bt_listener_.btree());
    bt_.bind_listener(bt_listener_);

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);
}

void SkyFish::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    this->set_enemy_state(EnemyState::Despawning);

    blink_animator_.start();
}

void SkyFish::update_movement() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {
        const auto behavior_state = bt_listener_.behavior_state();

        if ( behavior_state == SkyFishBehaviorState::Chase ) {
            
            auto real_pos = this->precise_position();

            const float stiffness = 0.03f;
            const float damping = 1.0f;
            auto player_pos = player_.position().template cast_to<float>();

            auto delta = player_pos - real_pos;
            auto force = delta * stiffness - velocity_ * damping;

            velocity_ = velocity_ + force + force_ex_;
            real_pos += velocity_;

            force_ex_ *= 0.5;

            this->set_precise_position(real_pos);

        } else {
            if ( behavior_state == SkyFishBehaviorState::LookRight ) {
                at(this->mut_hitboxes(), EnemyHitboxIndex::ViewRight).set_enabled(true);
                at(this->mut_hitboxes(), EnemyHitboxIndex::ViewLeft).set_enabled(false);
            } else if (behavior_state == SkyFishBehaviorState::LookLeft ) {
                at(this->mut_hitboxes(), EnemyHitboxIndex::ViewRight).set_enabled(false);
                at(this->mut_hitboxes(), EnemyHitboxIndex::ViewLeft).set_enabled(true);
            } else { }
        }

        bt_listener_.clear_all_hit_flags();

    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }

    } else { }
}

void SkyFish::update_animation() {

    if ( this->enemy_state() == EnemyState::Inactive ) return;

    if ( bt_.has_finished() ) {
        bt_.reset_state();
    }

    bt_.proc_until_blocked(false);

    auto next_state = anim_state_;

    const auto behavior_state = bt_listener_.behavior_state();

    switch ( behavior_state ) {
    case SkyFishBehaviorState::Chase:
        
        if ( velocity_.x > 0 && velocity_.y > 0 ) {
            next_state = SkyFishAnimState::FallRight;
        } else if ( velocity_.x > 0 && velocity_.y <= 0 ) {
            next_state = SkyFishAnimState::RiseRight;
        } else if ( velocity_.x <= 0 && velocity_.y > 0 ) {
            next_state = SkyFishAnimState::FallLeft;
        } else if ( velocity_.x <= 0 && velocity_.y <= 0 ) {
            next_state = SkyFishAnimState::RiseLeft;
        } else {
        }
        break;
    case SkyFishBehaviorState::LookRight:
        next_state = SkyFishAnimState::HoverRight;
        break;
    case SkyFishBehaviorState::LookLeft:
        next_state = SkyFishAnimState::HoverLeft;
        break;
    default:
        break;
    }
    
    if ( anim_state_ != next_state ) {
        anim_state_ = next_state;
        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();
    }

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

int32_t SkyFish::apply_damage_to(
    Player& player,
    size_t enemy_hitbox_index
) const {
    int32_t amount = 0;
    if ( enemy_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body) ) {
        amount = 1;
        player.receive_damage(amount);
        if ( this->is_direction_right() ) {
            player.receive_impact({12.0f, 0.0f});
        } else {
            player.receive_impact({-12.0f, 0.0f});
        }
    }
    return amount;
}

void SkyFish::receive_damage(int32_t amount) {
    if ( this->hp() > amount ) {
        this->set_hp(hp() - amount);
    } else {
        this->set_hp(0);
    }
}

void SkyFish::receive_impact(mgc::math::Vec2f delta) {
    force_ex_ += delta;
}

void SkyFish::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) { 
    bt_listener_.set_hit_flag(info.self_hitbox_index);
}

void SkyFish::on_attack_hit(
    const Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {
        
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);

        sound_.play_sound_effect(MML_SE_3_DAMAGE, 0.0);
    }
}

void SkyFish::on_collision_resolved(
    const stage::LayerBlock& layer,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( info.obj_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {
        auto pos = this->position();
        pos += info.pushback;

        this->set_position(pos);

        if ( info.pushback.x > 0 ) {
            force_ex_.x += 2;
        } else if ( info.pushback.x < 0 ) {
            force_ex_.x -= 2;
        } else { }

        if ( info.pushback.y > 0 ) {
            force_ex_.y += 2;
        } else if ( info.pushback.y < 0 ) {
            force_ex_.y -= 2;
        } else { }
    }
}


}// namespace enemy
}// namespace app

