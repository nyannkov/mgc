#include "walker.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

Walker::Walker(
    const FrameTimerT& timer,
    SoundControllerT& sound
) : sound_(sound),
    anim_(timer), 
    sw_(timer),
    force_ex_({0.0f, 0.0f}),
    anim_state_(WalkerAnimState::StandRight),
    blink_animator_(timer) {

    blink_animator_.set_target(*this);
    set_hp(WalkerMaxHP);
    set_full_hp(WalkerMaxHP);
    set_money(10);
}

void Walker::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    
    spawn(pos, is_right, WalkerMode::Normal);
}

void Walker::spawn(const mgc::math::Vec2i& pos, bool is_right, WalkerMode mode) {
    set_hp(WalkerMaxHP);
    this->set_enemy_state(EnemyState::Spawning);

    if ( is_right ) {
        anim_state_ = WalkerAnimState::StandRight;
    } else {
        anim_state_ = WalkerAnimState::StandLeft;
    }
    anim_.set_anim_frames(get_anim_frames(anim_state_));
    anim_.start_animation();
    anim_.set_loop(true);
    anim_.set_current_frame(this->mut_sprite());
    this->set_position(pos);

    auto& hitboxes = this->mut_hitboxes();

    // body
    auto& hitbox_body = at(hitboxes, EnemyHitboxIndex::Body);
    hitbox_body.set_offset({0, 0});
    hitbox_body.set_size({16, 16});
    hitbox_body.set_enabled(true);

    this->set_visible(true);
    mode_ = mode;

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);

    sw_.restart();
}

void Walker::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    this->set_enemy_state(EnemyState::Despawning);

    blink_animator_.start();
}

void Walker::update_movement() {
    if ( mode_ == WalkerMode::Normal ) {
        update_movement_normal();
    } else {
        update_movement_dancing();
    }
}

void Walker::update_movement_dancing() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {

        auto pos = this->precise_position();
        if ( velocity_.y < (MGC_CELL_LEN-1) ) {
            velocity_.y += 1.0f;
        }
        pos += velocity_ + force_ex_;
        force_ex_ *= dumping_rate_;
        dumping_rate_ -= 0.02f;
        if ( dumping_rate_ < 0 ) {
            dumping_rate_ = 0.0f;
        }
        this->set_precise_position(pos);

    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }

    } else { }

}

void Walker::update_movement_normal() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {

        auto pos = this->precise_position();
        if ( anim_state_ == WalkerAnimState::WalkRight ) {
            pos.x += 1;
        } else if ( anim_state_ == WalkerAnimState::WalkLeft ) {
            pos.x -= 1;
        } else { }

        if ( velocity_.y < (MGC_CELL_LEN-1) ) {
            velocity_.y += 1.0f;
        }
        pos.y += velocity_.y + force_ex_.y;
        pos.x += velocity_.x + force_ex_.x;
        force_ex_ *= dumping_rate_;
        dumping_rate_ -= 0.02f;
        if ( dumping_rate_ < 0 ) {
            dumping_rate_ = 0.0f;
        }

        this->set_precise_position(pos);

    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }

    } else { }
}

void Walker::update_animation() {

    if ( this->enemy_state() == EnemyState::Inactive ) return;


    if ( mode_ == WalkerMode::Normal ) {
        if ( sw_.elapsed_ms() >= 3000 ) {
            sw_.restart();
            is_walking_ = !is_walking_;
            if ( (rand()%2) != 0 ) {
                if ( is_walking_ ) {
                    anim_state_ = WalkerAnimState::WalkRight;
                } else {
                    anim_state_ = WalkerAnimState::StandRight;
                }
            } else {
                if ( is_walking_ ) {
                    anim_state_ = WalkerAnimState::WalkLeft;
                } else {
                    anim_state_ = WalkerAnimState::StandLeft;
                }
            }

            anim_.set_loop(true);
            anim_.set_anim_frames(get_anim_frames(anim_state_));
            anim_.start_animation();
        }

    } else {
        auto pos = this->precise_position();
        uint32_t count = sound_.update_bgm_param_count();
        uint32_t param = sound_.last_bgm_param();

        if ( update_bgm_param_count_ != count ) {
            update_bgm_param_count_ = count;
            if ( (count & 0x2) == 0 ) {
                anim_state_ = WalkerAnimState::KickRight;
                if ( (count & 0x1) == 0) {
                    velocity_.y -= 5;
                    if ( param >= 5 ) {
                        velocity_.x = 1;
                    }
                }
            } else {
                anim_state_ = WalkerAnimState::KickLeft;
                if ( (count & 0x1) == 0) {
                    velocity_.y -= 5;
                    if ( param >= 5 ) {
                        velocity_.x = -1;
                    }
                }
            }
            anim_.set_loop(false);
            anim_.set_anim_frames(get_anim_frames(anim_state_));
            anim_.start_animation();
        }
    }

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

int32_t Walker::apply_damage_to(
    Player& player,
    size_t enemy_hitbox_index
) const {
    int32_t amount = 0;
    if ( enemy_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body) ) {
        amount = 1;
        player.receive_damage(amount);
        if ( this->is_direction_right() ) {
            player.receive_impact({3.0f, 0.0f});
        } else {
            player.receive_impact({-3.0f, 0.0f});
        }
    }
    return amount;
}

void Walker::receive_damage(int32_t amount) {
    if ( this->hp() > amount ) {
        this->set_hp(hp() - amount);
    } else {
        this->set_hp(0);
    }
}

void Walker::receive_impact(mgc::math::Vec2f delta, float dumping_rate) {
    force_ex_ += delta;
    dumping_rate_ = dumping_rate;
}

void Walker::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) { 
}

void Walker::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {
        
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);
    }
}

void Walker::on_collision_resolved(
    const stage::LayerBlock& layer,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( info.obj_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {

        if ( info.pushback.y < 0 ) {
            if ( velocity_.y > 0 ) {
                velocity_.y = 0.0f;
            }
            velocity_.x = 0;
        }
        
        auto pos = this->position();
        pos += info.pushback;

        this->set_position(pos);
    }
}

bool Walker::draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const {
    
    return this->sprite().draw(fb, cam_pos, options);
}



}// namespace enemy
}// namespace app

