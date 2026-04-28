#include "floaty_fish.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

FloatyFish::FloatyFish(
    const FrameTimerT& timer,
    SoundControllerT& sound
) : sound_(sound),
    anim_(timer), 
    sw_(timer),
    force_ex_({0.0f, 0.0f}),
    anim_state_(FloatyFishAnimState::FloatingRight),
    blink_animator_(timer) {

    blink_animator_.set_target(*this);
    set_hp(FloatyFishMaxHP);
    set_full_hp(FloatyFishMaxHP);
    set_money(10);
}

void FloatyFish::spawn(const mgc::math::Vec2i& pos) {
    
    spawn(pos, true);
}

void FloatyFish::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    set_hp(FloatyFishMaxHP);
    this->set_enemy_state(EnemyState::Spawning);

    if ( is_right ) {
        anim_state_ = FloatyFishAnimState::FloatingRight;
    } else {
        anim_state_ = FloatyFishAnimState::FloatingLeft;
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

    auto& hitbox_head = at(hitboxes, EnemyHitboxIndex::Head);
    hitbox_head.set_offset({0, 8});
    hitbox_head.set_size({16, 1});
    hitbox_head.set_enabled(true);

    this->set_visible(true);

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);

    sw_.restart();
}

void FloatyFish::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    this->set_enemy_state(EnemyState::Despawning);

    blink_animator_.start();
}

void FloatyFish::update_movement() {
    update_movement_normal();
}


void FloatyFish::update_movement_normal() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {

        auto pos = this->precise_position();
        if ( anim_state_ == FloatyFishAnimState::FloatingRight ) {
            pos.x += 1;
        } else if ( anim_state_ == FloatyFishAnimState::FloatingLeft ) {
            pos.x -= 1;
        } else { }

        if ( hit_water_ ) {
            if ( hit_head_water_ ) {
                hit_head_water_ = false;
                if ( velocity_.y > -3 ) {
                    velocity_.y -= 0.5f;
                }
                velocity_.y *= 0.9f;
            } else {
                if ( velocity_.y > 3 ) {
                    velocity_.y += 0.1f;
                }
            }
        } else {
            if ( velocity_.y < (MGC_CELL_LEN-1) ) {
                velocity_.y += 1.0f;
            }
        }
        pos.y += velocity_.y;

        this->set_precise_position(pos);

        hit_water_ = false;
        hit_head_water_ = false;

    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }

    } else { }
}

void FloatyFish::update_animation() {

    if ( this->enemy_state() == EnemyState::Inactive ) return;

    if ( sw_.elapsed_ms() >= 3000 ) {
        sw_.restart();
        is_walking_ = !is_walking_;
        if ( (rand()%2) != 0 ) {
            if ( is_walking_ ) {
                anim_state_ = FloatyFishAnimState::FloatingRight;
            } else {
                anim_state_ = FloatyFishAnimState::FloatingLeft;
            }
        }

        anim_.set_loop(true);
        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();
    }

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

int32_t FloatyFish::apply_damage_to(
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

void FloatyFish::receive_damage(int32_t amount) {
    if ( this->hp() > amount ) {
        this->set_hp(hp() - amount);
    } else {
        this->set_hp(0);
    }
}

void FloatyFish::receive_impact(mgc::math::Vec2f delta) {
    force_ex_ += delta;
}

void FloatyFish::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) { 
}

void FloatyFish::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {
        
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);

        sound_.play_sound_effect(MML_SE_3_DAMAGE);
    }
}

void FloatyFish::on_collision_resolved(
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

void FloatyFish::on_collision_resolved(
    const stage::LayerWater& water,
    const mgc::collision::MapPushbackInfo& info
) { 
    if ( info.obj_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {
        hit_water_ = true;
    } else if ( info.obj_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Head)) {
        hit_head_water_ = true;
    }
}

}// namespace enemy
}// namespace app

