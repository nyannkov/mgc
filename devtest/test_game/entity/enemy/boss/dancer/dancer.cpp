#include "dancer.hpp"
#include "dancer_anim.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

Dancer::Dancer(
    const FrameTimerT& timer,
    SoundControllerT& sound
) : sound_(sound),
    anim_(timer), 
    sw_(timer),
    force_ex_({0.0f, 0.0f}),
    anim_state_(DancerAnimState::StaticYeah),
    blink_animator_(timer) {

      blink_animator_.set_target(*this);
      blink_animator_.set_blink_half_period(50);
      blink_animator_.set_blink_count_max(40);
      set_hp(DancerMaxHP);
      set_full_hp(DancerMaxHP);
      set_money(10);
}

void Dancer::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    
    this->set_enemy_state(EnemyState::Spawning);

    //anim_state_ = DancerAnimState::StaticYeah;
    anim_state_ = DancerAnimState::WalkLeft;
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
    hitbox_core.set_offset({0, 0});
    hitbox_core.set_size({16, 16});
    hitbox_core.set_enabled(true);

    // No spawning animation
    this->set_enemy_state(EnemyState::Active);

    sw_.restart();
}

void Dancer::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    this->set_enemy_state(EnemyState::Despawning);
    blink_animator_.set_end_state(mgc::utils::BlinkEndState::Hidden);

    blink_animator_.set_blink_half_period(50);
    blink_animator_.set_blink_count_max(40);
    blink_animator_.start();
}

void Dancer::update_movement() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {
        
        auto pos = this->precise_position();
        pos += velocity_;
        velocity_.x *= 0.4;
        velocity_.y += 0.5;
        this->set_precise_position(pos);

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

void Dancer::update_animation() {

    if ( this->enemy_state() == EnemyState::Inactive ) return;

    if ( this->enemy_state() == EnemyState::Despawning ) {
        anim_state_ = DancerAnimState::StaticYeah;
        anim_.set_anim_frames(get_anim_frames(anim_state_));
        anim_.start_animation();
        anim_.set_loop(true);
    } else {

        auto pos = this->precise_position();
        uint32_t count = sound_.update_bgm_param_count();
        uint32_t param = sound_.last_bgm_param();
        if ( update_bgm_param_count_ != count ) {
            update_bgm_param_count_ = count;

            switch (param) {
            case 0:
                anim_state_ = DancerAnimState::StaticYeah;
                break;
                
            case 1:
                if ( rand()%2 == 0 ) {
                    velocity_.x = -8;
                    anim_state_ = DancerAnimState::WalkLeft;
                } else {
                    velocity_.x = 8;
                    anim_state_ = DancerAnimState::WalkRight;
                }
                break;
            case 2:
                if ( pos.x > MGC_CELL2PIXEL(30) ) {
                    velocity_.x = -8;
                    anim_state_ = DancerAnimState::WalkLeft;
                } else {
                    velocity_.x = 8;
                    anim_state_ = DancerAnimState::WalkRight;
                }
                break;
            case 3:
                anim_state_ = DancerAnimState::StaticYeah;
                break;
            case 4:
                anim_state_ = DancerAnimState::Yeah;
                break;
            case 5://FALLTHROUGH
            case 6:
                anim_state_ = DancerAnimState::Boxing;
                if ( rand()%2 == 0 ) {
                    velocity_.x = -8;
                } else {
                    velocity_.x = 8;
                }
                break;
            default:
                anim_state_ = DancerAnimState::StaticYeah;
                break;
            }

            anim_.set_anim_frames(get_anim_frames(anim_state_));
            anim_.start_animation();
            anim_.set_loop(false);
        }
    }
    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

int32_t Dancer::apply_damage_to(
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

void Dancer::receive_damage(int32_t amount) {
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

void Dancer::receive_impact(mgc::math::Vec2f delta, float dumping_rate) {
    force_ex_ += delta;
}

void Dancer::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) { 
}

void Dancer::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( info.self_hitbox_index == static_cast<size_t>(EnemyHitboxIndex::ViewLeft)) { //TODO rename
        size_t attack_hitbox_index = info.other_hitbox_index;

        attack.apply_damage_to(*this, attack_hitbox_index);
    }
}

void Dancer::on_collision_resolved(
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
    }
}


}// namespace enemy
}// namespace app

