#include "rocker.hpp"
#include "rocker_anim.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

Rocker::Rocker(
    const FrameTimerT& timer,
    SoundControllerT& sound
) : sound_(sound),
    anim_(timer), 
    sw_(timer),
    force_ex_({0.0f, 0.0f}),
    anim_state_(RockerAnimState::Stand2),
    blink_animator_(timer) {

      blink_animator_.set_target(*this);
      blink_animator_.set_blink_half_period(50);
      blink_animator_.set_blink_count_max(40);
      set_hp(RockerMaxHP);
      set_full_hp(RockerMaxHP);
      set_money(10);
}

void Rocker::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    
    this->set_enemy_state(EnemyState::Spawning);

    state_ = RockerState::Stopped;

    anim_state_ = RockerAnimState::Stand2;
    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();
    anim_.set_loop(false);
    anim_.set_current_frame(this->mut_sprite());
    this->set_position(pos);
    shadow_mode_ = false;
    for ( size_t i = 0; i < countof(color_palette_); i++ ) {
        color_palette_[i] = MGC_COLOR(0.1, 0.1, 0.1);
    }

    auto& hitboxes = this->mut_hitboxes();

    // body
    auto& hitbox_body = at(hitboxes, EnemyHitboxIndex::Body);
    hitbox_body.set_offset({0, 0});
    hitbox_body.set_size({16, 32});
    hitbox_body.set_enabled(true);

    // TODO core
    auto& hitbox_core = at(hitboxes, EnemyHitboxIndex::ViewLeft);//TODO rename
    hitbox_core.set_offset({0, 0});
    hitbox_core.set_size({16, 16});
    hitbox_core.set_enabled(true);

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);

    sw_.restart();
}

void Rocker::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    this->set_enemy_state(EnemyState::Despawning);
    blink_animator_.set_end_state(mgc::utils::BlinkEndState::Hidden);

    blink_animator_.set_blink_half_period(50);
    blink_animator_.set_blink_count_max(40);
    blink_animator_.start();
}

void Rocker::update_movement() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {
        
        auto pos = this->precise_position();
        pos += velocity_;
        velocity_.y += 1;
        this->set_precise_position(pos);

        blink_animator_.update();
        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {
            blink_animator_.clear();
        }

    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }

    } else { }
}

void Rocker::update_animation() {

    if ( this->enemy_state() == EnemyState::Inactive ) return;

    if ( this->enemy_state() == EnemyState::Despawning ) {
        anim_state_ = RockerAnimState::Singing;
        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();
        anim_.set_loop(false);
    } else {

        auto pos = this->precise_position();
        uint32_t count = sound_.update_bgm_param_count();
        uint32_t param = sound_.last_bgm_param();

        if ( update_bgm_param_count_ != count ) {
            update_bgm_param_count_ = count;

            switch (param) {
            case 100:
                anim_state_ = RockerAnimState::Waving;
                break;
            case 101:
                anim_state_ = RockerAnimState::StopWaving;
                break;
            case 102:
                anim_state_ = RockerAnimState::Playing;
                break;

            case 1:
                anim_state_ = RockerAnimState::Playing;
                state_ = RockerState::Playing;
                break;

             case 2:
                anim_state_ = RockerAnimState::Singing;
                state_ = RockerState::Singing;
                break;

             case 4:
                if ( velocity_.y >= 0 ) {
                    velocity_.y -= 1;
                }
                if ( state_ != RockerState::StepBackward &&
                     state_ != RockerState::StepForward
                ) {
                    state_ = RockerState::StepBackward;
                    anim_state_ = RockerAnimState::WidePlaying;
                }

                if ( state_ == RockerState::StepForward ) {
                    if ( this->position().x >= MGC_CELL2PIXEL(21) ) {
                        state_ = RockerState::StepBackward;
                        anim_state_ = RockerAnimState::WidePlaying;
                    } else {
                        velocity_.x = 1;
                    }
                } else {
                    if ( this->position().x <= MGC_CELL2PIXEL(17) ) {
                        state_ = RockerState::StepForward;
                        anim_state_ = RockerAnimState::Stepping;
                    } else {
                        velocity_.x = -1;
                    }
                }
                break;

            default:
                break;
            }

            anim_.set_anim_frames(get_anim_frames(anim_state_));
            anim_.start_animation();
            if ( anim_state_ == RockerAnimState::Waving ) {
                anim_.set_loop(true);
            } else {
                anim_.set_loop(false);
            }
        }
    }
    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

int32_t Rocker::apply_damage_to(
    Player& player,
    size_t enemy_hitbox_index
) const {
    int32_t amount = 0;
    //if ( enemy_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body) ) {
    //    amount = 1;
    //    player.receive_damage(amount);
    //}
    return amount;
}

void Rocker::receive_damage(int32_t amount) {
    // invincible
    
    //blink_animator_.set_blink_half_period(100);
    //blink_animator_.set_blink_count_max(5);
    //blink_animator_.set_end_state(mgc::utils::BlinkEndState::Visible);
    //blink_animator_.start();
    //if ( this->hp() > amount ) {
    //    this->set_hp(hp() - amount);
    //} else {
    //    this->set_hp(0);
    //}
}

void Rocker::receive_impact(mgc::math::Vec2f delta, float dumping_rate) {
    //force_ex_ += delta;
}

void Rocker::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) { 
}

void Rocker::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::ViewLeft)) {
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);
    }
}

void Rocker::on_collision_resolved(
    const stage::LayerBlock& layer,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( info.obj_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {

        if ( info.pushback.y < 0 ) {
            if ( velocity_.y > 0 ) {
                velocity_.y = 0.0f;
                velocity_.x = 0.0f;
            }
        }
        
        auto pos = this->position();
        pos += info.pushback;

        this->set_position(pos);
    }
}

bool Rocker::draw(
    mgc::graphics::Framebuffer &fb,
    const mgc::math::Vec2i &cam_pos,
    const mgc::graphics::DrawOptions *options
) const {
    
    if ( shadow_mode_ ) {
        mgc::graphics::DrawOptions opt = {0};
        opt.eff_flags = static_cast<uint16_t>(DRAW_EFFECT_PALETTE_CHANGE);
        opt.palette_array = color_palette_;
        opt.palette_count = countof(color_palette_);

        return Enemy::draw(fb, cam_pos, &opt);
    } else {
        return Enemy::draw(fb, cam_pos, nullptr);
    }
    
}


}// namespace enemy
}// namespace app

