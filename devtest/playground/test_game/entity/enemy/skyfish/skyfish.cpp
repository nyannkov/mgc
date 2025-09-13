#include "skyfish.hpp"
#include "entity/player/player.hpp"
#include "skyfish_hitbox.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

SkyFish::SkyFish(
    const FrameTimerT& frame_timer,
    SoundControllerT& sound_controller,
    const Player& player,
    PlayerStats& player_stats
) : player_(player),
    sound_controller_(sound_controller),
    player_stats_(player_stats),
    anim_(frame_timer), 
    bt_(frame_timer),
    bt_listener_(frame_timer),
    velocity_({0.0f, 0.0f}),
    force_ex_({0.0f, 0.0f}),
    anim_state_(SkyFishAnimState::HoverRight),
    blink_animator_(frame_timer) {

      blink_animator_.set_target(*this);
      set_hp(SkyFishMaxHP);
      set_full_hp(SkyFishMaxHP);
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
    anim_.set_current_frame(this->sprite());
    this->set_position(pos);

    velocity_ = {0.0f, 0.0f};

    auto& hitboxes = this->hitboxes();

    // body
    auto& hitbox_body = at(hitboxes, SkyFishHitboxIndex::Body);
    hitbox_body.id = static_cast<mgc_id_t>(SkyFishHitboxIndex::Body);
    hitbox_body.offset = mgc::collision::HitboxOffset(0, 0);
    hitbox_body.size = mgc::collision::HitboxSize(16, 16);
    hitbox_body.enabled = true;

    // view left
    auto& hitbox_view_left = at(hitboxes, SkyFishHitboxIndex::ViewLeft);
    hitbox_view_left.id = static_cast<mgc_id_t>(SkyFishHitboxIndex::ViewLeft);
    hitbox_view_left.offset = mgc::collision::HitboxOffset(8-16*7, 8-16*5);
    hitbox_view_left.size = mgc::collision::HitboxSize(16*7, 16*10);
    hitbox_view_left.enabled = true;

    // view right
    auto& hitbox_view_right = at(hitboxes, SkyFishHitboxIndex::ViewRight);
    hitbox_view_right.id = static_cast<mgc_id_t>(SkyFishHitboxIndex::ViewRight);
    hitbox_view_right.offset = mgc::collision::HitboxOffset(8, 8-16*5);
    hitbox_view_right.size = mgc::collision::HitboxSize(16*7, 16*10);
    hitbox_view_right.enabled = false;

    // Behavior Tree
    bt_listener_.clear_all_hit_flags();
    bt_.set_btree(bt_listener_.btree());
    bt_.bind_listener(bt_listener_);

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);
}

void SkyFish::despawn() {

    auto& hitboxes = this->hitboxes();
    for ( auto& h : hitboxes ) { h.enabled = false; }

    this->set_enemy_state(EnemyState::Despawning);

    blink_animator_.start();
}

void SkyFish::pre_update() {

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
                at(hitboxes(), SkyFishHitboxIndex::ViewRight).enabled = true;
                at(hitboxes(), SkyFishHitboxIndex::ViewLeft).enabled = false;
            } else if (behavior_state == SkyFishBehaviorState::LookLeft ) {
                at(hitboxes(), SkyFishHitboxIndex::ViewRight).enabled = false;
                at(hitboxes(), SkyFishHitboxIndex::ViewLeft).enabled = true;
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

void SkyFish::post_update() {

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
    anim_.set_current_frame(this->sprite());
}

int32_t SkyFish::apply_damage_to(
    Player& player,
    size_t enemy_hitbox_index
) const {
    int32_t amount = 0;
    if ( enemy_hitbox_index == static_cast<size_t>(SkyFishHitboxIndex::Body) ) {
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
    if ( info.self_hitbox_index == static_cast<size_t>(SkyFishHitboxIndex::Body)) {
        
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);

        sound_controller_.play_sound_effect(MML_SE_3_DAMAGE, 0.0);

        if ( this->hp() <= 0 ) {
            player_stats_.add_gold(10);
            this->despawn();
        }
    }
}

void SkyFish::on_collision_resolved(
    const CollisionTileLayer& layer,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( info.obj_hitbox_index == static_cast<size_t>(SkyFishHitboxIndex::Body)) {
        auto pos = this->position();
        pos += info.pushback;

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

        this->set_position(pos);
    }
}


}// namespace enemy
}// namespace app

