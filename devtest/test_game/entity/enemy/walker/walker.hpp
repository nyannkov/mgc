#ifndef MGC_ENEMY_WALKER_HPP
#define MGC_ENEMY_WALKER_HPP

#include "app_common.hpp"
#include "entity/enemy/enemy.hpp"
#include "walker_anim.hpp"

namespace app {
namespace enemy {

constexpr int32_t WalkerMaxHP = 2;
enum class WalkerMode {
    Normal,
    Dancing
};

struct Walker: Enemy {
    Walker(
        const FrameTimerT& timer,
        SoundControllerT& sound
    );
    ~Walker() = default;
    Walker(const Walker&) = delete;
    Walker& operator=(const Walker&) = delete;
    Walker(Walker&&) = default;
    Walker& operator=(Walker&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void spawn(const mgc::math::Vec2i& pos, bool is_right, WalkerMode mode);
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    int32_t apply_damage_to(
        Player& player,
        size_t enemy_hitbox_index
    ) const override;
    void receive_damage(int32_t amount) override;
    void receive_impact(mgc::math::Vec2f delta) override;

    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) override;

private:
    SoundControllerT& sound_;
    AnimControllerT anim_;
    mgc::math::Vec2f force_ex_;
    mgc::math::Vec2f velocity_;
    WalkerAnimState anim_state_;
    BlinkAnimatorT blink_animator_;
    StopwatchT sw_;
    WalkerMode mode_ = WalkerMode::Normal;
    uint32_t update_bgm_param_count_ = 0;
    bool is_walking_ = false;
    bool is_direction_right() const {
        return (
            ( anim_state_ == WalkerAnimState::StandRight ) ||
            ( anim_state_ == WalkerAnimState::WalkRight ) ||
            ( anim_state_ == WalkerAnimState::KickRight )
        );
    }
    void update_movement_normal();
    void update_movement_dancing();
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_WALKER_HPP*/

