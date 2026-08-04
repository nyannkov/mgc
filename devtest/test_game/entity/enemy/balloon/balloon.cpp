#include "balloon.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

Balloon::Balloon(
    const FrameTimerT& timer,
    SoundControllerT& sound,
    const Player& player
) : sound_(sound),
    anim_(timer), 
    sw_(timer),
    force_ex_({0.0f, 0.0f}),
    anim_state_(BalloonAnimState::FlutterRight),
    blink_animator_(timer) {

    blink_animator_.set_target(*this);
    set_hp(BalloonMaxHP);
    set_full_hp(BalloonMaxHP);
    set_money(10);
}

void Balloon::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    spawn(
        pos,
        is_right,
        pos.x,
        pos.x+16*10
    );
}

void Balloon::spawn(const mgc::math::Vec2i& pos, bool is_right, mgc_world_t min_x, mgc_world_t max_x) {

    orig_pos_ = pos;
    
    set_hp(BalloonMaxHP);
    this->set_enemy_state(EnemyState::Spawning);

    if ( is_right ) {
        anim_state_ = BalloonAnimState::FlutterRight;
    } else {
        anim_state_ = BalloonAnimState::FlutterLeft;
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

    // view
    auto& hitbox_view = at(hitboxes, EnemyHitboxIndex::ViewBottom);
    if ( is_right ) {
        hitbox_view.set_offset({0, 0});
    } else {
        hitbox_view.set_offset({static_cast<int16_t>(-VIEW_RANGE_X), 0});
    }
    hitbox_view.set_size({static_cast<uint16_t>(VIEW_RANGE_X), 16*8});
    hitbox_view.set_enabled(true);

    this->set_visible(true);

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);

    state_ = State::Fluttering;

    min_x_ = min_x;
    max_x_ = max_x;
    
    hit_wall_ = false;

    sw_.restart();
}

void Balloon::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    this->set_enemy_state(EnemyState::Despawning);

    blink_animator_.start();
}

void Balloon::update_movement() {
    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {
        
        auto pos = this->precise_position();

        if ( state_ == State::Fluttering ) {

            velocity_.y = 0;
            if ( anim_state_ == BalloonAnimState::FlutterRight ) {
                velocity_.x = 1;
                pos += velocity_;

            } else if ( anim_state_ == BalloonAnimState::FlutterLeft ) {
                velocity_.x = -1;
                pos += velocity_;
            } else { }

        } else if ( state_ == State::Falling ) {

            if ( velocity_.y < (MGC_CELL_LEN-1) ) {
                velocity_.y += 1.0f;
            }
            pos += velocity_;

        } else if ( state_ == State::Rising ) {
            velocity_.x = 0;
            velocity_.y = -1;
            pos += velocity_;
            if ( pos.y <= orig_pos_.y ) {
                state_ = State::Fluttering;
            }

        } else { }

        this->set_precise_position(pos);

    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }

    } else { }
}

void Balloon::update_animation() {

    if ( this->enemy_state() == EnemyState::Inactive ) return;

    auto next_ = anim_state_;
    const auto pos = this->precise_position();

    if ( state_ == State::Fluttering ) {

        if ( is_direction_right() ) {
            if ( hit_wall_ || pos.x >= max_x_ ) {
                next_ = BalloonAnimState::FlutterLeft;
            }
        } else {
            if ( hit_wall_ || pos.x <= min_x_ ) {
                next_ = BalloonAnimState::FlutterRight;
            }
        }

        hit_wall_ = false;

    } else if ( state_ == State::Falling ) {
        if ( is_direction_right() ) {
            next_ = BalloonAnimState::FallRight;
        } else {
            next_ = BalloonAnimState::FallLeft;
        }
    } else if ( state_ == State::Rising ) {
        if ( is_direction_right() ) {
            next_ = BalloonAnimState::FlutterRight;
        } else {
            next_ = BalloonAnimState::FlutterLeft;
        }
    } else { }

    auto& hitbox_view = at(this->mut_hitboxes(), EnemyHitboxIndex::ViewBottom);
    if ( is_direction_right() ) {
        hitbox_view.set_offset({0, 0});
    } else {
        hitbox_view.set_offset({static_cast<int16_t>(-VIEW_RANGE_X), 0});
    }

    if ( anim_state_ != next_ ) {
        anim_state_ = next_;
        anim_.set_loop(true);
        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();
    }
    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());

    if ( sw_.elapsed_ms() >= 3000 ) {
        sw_.restart();
    }
}

int32_t Balloon::apply_damage_to(
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

void Balloon::receive_damage(int32_t amount) {
    if ( this->hp() > amount ) {
        this->set_hp(hp() - amount);
    } else {
        this->set_hp(0);
    }
}

void Balloon::receive_impact(mgc::math::Vec2f delta) {
    force_ex_ += delta;
}

void Balloon::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::ViewBottom)) {
        if ( state_ == State::Fluttering ) {
            state_ = State::Falling;
        }
    }
}

void Balloon::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {
        
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);

        sound_.play_sound_effect(MML_SE_3_DAMAGE);
    }
}

void Balloon::on_collision_resolved(
    const stage::LayerBlock& layer,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( info.obj_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::Body)) {

        if ( info.pushback.y < 0 ) {
            if ( velocity_.y > 0 ) {
                velocity_.y = 0.0f;
                if ( state_ == State::Falling ) {
                    state_ = State::Rising;
                }
            }
            velocity_.x = 0;
        }

        if ( info.pushback.x != 0 ) {
            hit_wall_ = true;
        } 
        
        auto pos = this->position();
        pos += info.pushback;

        this->set_position(pos);
    }
}


}// namespace enemy
}// namespace app

