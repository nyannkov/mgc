#ifndef MGC_ENEMY_SPECTATOR_HPP
#define MGC_ENEMY_SPECTATOR_HPP

#include "app_common.hpp"
#include "entity/enemy/enemy.hpp"
#include "spectator_anim.hpp"

namespace app {
namespace enemy {

constexpr int32_t SpectatorMaxHP = 999999;
enum class SpectatorMode {
    Stand,
    Jump
};
enum class SpectatorType {
    Type1,
    Type2,
    Type3
};

struct Spectator: Enemy {
    Spectator(
        const FrameTimerT& timer,
        SoundControllerT& sound
    );
    ~Spectator() = default;
    Spectator(const Spectator&) = delete;
    Spectator& operator=(const Spectator&) = delete;
    Spectator(Spectator&&) = default;
    Spectator& operator=(Spectator&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void spawn(
        const mgc::math::Vec2i& pos,
        bool is_right,
        SpectatorMode mode,
        SpectatorType type
    );
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    int32_t apply_damage_to(
        Player& player,
        size_t enemy_hitbox_index
    ) const override;

    void receive_damage(int32_t amount) override { };
    void receive_impact(mgc::math::Vec2f delta, float dumping_rate) { };

    void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) override;

    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override;

    auto jump_check_interval() const { return jump_check_interval_; }
    void set_jump_check_interval(StopwatchT::timestamp_t interval) {
        jump_check_interval_ = interval;
    }

private:
    SoundControllerT& sound_;
    AnimControllerT anim_;
    float dumping_rate_ = 0.5f;
    mgc::math::Vec2f force_ex_;
    mgc::math::Vec2f velocity_;
    SpectatorAnimState anim_state_;
    BlinkAnimatorT blink_animator_;
    StopwatchT sw_;
    StopwatchT::timestamp_t jump_check_interval_ = 1000;
    SpectatorMode mode_ = SpectatorMode::Stand;
    SpectatorType type_ = SpectatorType::Type1;
    bool is_direction_right() const {
        return (
            ( anim_state_ == SpectatorAnimState::Type1_StandRight ) ||
            ( anim_state_ == SpectatorAnimState::Type2_StandRight ) ||
            ( anim_state_ == SpectatorAnimState::Type3_StandRight ) ||
            ( anim_state_ == SpectatorAnimState::Type1_JumpRight ) ||
            ( anim_state_ == SpectatorAnimState::Type2_JumpRight ) ||
            ( anim_state_ == SpectatorAnimState::Type3_JumpRight )
        );
    }

    SpectatorAnimState get_anim_state(
        bool is_right,
        SpectatorMode mode,
        SpectatorType type
    ) {
        SpectatorAnimState state;
        if ( mode == SpectatorMode::Stand ) {
            if ( is_right ) {
                state = (type == SpectatorType::Type1) ? SpectatorAnimState::Type1_StandRight
                      : (type == SpectatorType::Type2) ? SpectatorAnimState::Type2_StandRight
                      :                                  SpectatorAnimState::Type3_StandRight;
            } else {
                state = (type == SpectatorType::Type1) ? SpectatorAnimState::Type1_StandLeft
                      : (type == SpectatorType::Type2) ? SpectatorAnimState::Type2_StandLeft
                      :                                  SpectatorAnimState::Type3_StandLeft;
            }
        } else if ( mode == SpectatorMode::Jump ) {
            if ( is_right ) {
                state = (type == SpectatorType::Type1) ? SpectatorAnimState::Type1_JumpRight
                      : (type == SpectatorType::Type2) ? SpectatorAnimState::Type2_JumpRight
                      :                                  SpectatorAnimState::Type3_JumpRight;
            } else {
                state = (type == SpectatorType::Type1) ? SpectatorAnimState::Type1_JumpLeft
                      : (type == SpectatorType::Type2) ? SpectatorAnimState::Type2_JumpLeft
                      :                                  SpectatorAnimState::Type3_JumpLeft;
            }
        } else { 
            if ( is_right ) {
                state = SpectatorAnimState::Type1_StandRight;
            } else {
                state = SpectatorAnimState::Type1_StandLeft;
            }
        }

        return state;
    }
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_SPECTATOR_HPP*/

