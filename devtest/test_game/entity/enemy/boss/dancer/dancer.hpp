#ifndef MGC_ENEMY_DANCER_HPP
#define MGC_ENEMY_DANCER_HPP

#include "app_common.hpp"
#include "entity/enemy/enemy.hpp"
#include "dancer_anim.hpp"

namespace app {
namespace enemy {

constexpr int32_t DancerMaxHP = 30;

struct Dancer: Enemy {

    Dancer(
        const FrameTimerT& timer,
        SoundControllerT& sound
    );
    ~Dancer() = default;
    Dancer(const Dancer&) = delete;
    Dancer& operator=(const Dancer&) = delete;
    Dancer(Dancer&&) = default;
    Dancer& operator=(Dancer&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
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
    DancerAnimState anim_state_;
    uint32_t update_bgm_param_count_ = 0;
    BlinkAnimatorT blink_animator_;
    StopwatchT sw_;
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_DANCER_HPP*/

