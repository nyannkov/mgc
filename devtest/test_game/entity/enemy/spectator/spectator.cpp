#include "spectator.hpp"
#include "entity/player/player.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace enemy {

Spectator::Spectator(
    const FrameTimerT& timer,
    SoundControllerT& sound
) : sound_(sound),
    anim_(timer), 
    sw_(timer),
    force_ex_({0.0f, 0.0f}),
    anim_state_(SpectatorAnimState::Type1_StandRight),
    blink_animator_(timer) {

    blink_animator_.set_target(*this);
    set_hp(SpectatorMaxHP);
    set_full_hp(SpectatorMaxHP);
    set_money(10);
}

void Spectator::spawn(const mgc::math::Vec2i& pos, bool is_right) {
    
    //spawn(pos, is_right, SpectatorMode::Stand, SpectatorType::Type1);
    spawn(pos, is_right, SpectatorMode::Jump, SpectatorType::Type2);
}

void Spectator::spawn(const mgc::math::Vec2i& pos, bool is_right, SpectatorMode mode, SpectatorType type) {
    set_hp(SpectatorMaxHP);
    this->set_enemy_state(EnemyState::Spawning);

    anim_state_ = get_anim_state(is_right, mode, type);
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

void Spectator::despawn() {

    auto& hitboxes = this->mut_hitboxes();
    for ( auto& h : hitboxes ) { h.set_enabled(false); }

    this->set_enemy_state(EnemyState::Despawning);

    blink_animator_.start();
}


void Spectator::update_movement() {

    auto state = this->enemy_state();

    if ( state == EnemyState::Active ) {

        if ( mode_ == SpectatorMode::Stand ) {
            
        } else {
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
        }
    } else if ( state == EnemyState::Despawning ) {
       
        blink_animator_.update();

        if ( blink_animator_.state() == mgc::utils::BlinkAnimatorState::Done ) {

            blink_animator_.clear();

            this->set_enemy_state(EnemyState::Inactive);
        }

    } else { }

}

void Spectator::update_animation() {
    
    if ( this->enemy_state() == EnemyState::Inactive ) return;

    if ( mode_ == SpectatorMode::Jump ) {
        if ( sw_.elapsed_ms() >= jump_check_interval_ ) {
            sw_.restart();
                
            if ( (rand()%2) != 0 ) {
                velocity_.y -= 5;

                anim_.set_loop(false);
                anim_.set_anim_frames(get_anim_frames(anim_state_));
                anim_.start_animation();
            }
        }

        anim_.proc();
        anim_.set_current_frame(this->mut_sprite());
    }

}

int32_t Spectator::apply_damage_to(
    Player& player,
    size_t enemy_hitbox_index
) const {
    return 0;
}

void Spectator::on_collision_resolved(
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

bool Spectator::draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const {
    
    return this->sprite().draw(fb, cam_pos, options);
}



}// namespace enemy
}// namespace app

