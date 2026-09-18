#ifndef MGC_ENEMY_BALLOON_HPP
#define MGC_ENEMY_BALLOON_HPP

#include "app_common.hpp"
#include "entity/enemy/enemy.hpp"
#include "balloon_anim.hpp"

namespace app {
namespace enemy {

constexpr int32_t BalloonMaxHP = 2;

struct Balloon: Enemy {
    Balloon(
        const FrameTimerT& timer,
        SoundControllerT& sound,
        const Player& player
    );
    ~Balloon() = default;
    Balloon(const Balloon&) = delete;
    Balloon& operator=(const Balloon&) = delete;
    Balloon(Balloon&&) = default;
    Balloon& operator=(Balloon&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void spawn(const mgc::math::Vec2i& pos, bool is_right, mgc_world_t min_x, mgc_world_t max_x);
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    int32_t apply_damage_to(
        Player& player,
        size_t enemy_hitbox_index
    ) const override;
    void receive_damage(int32_t amount) override;
    void receive_impact(mgc::math::Vec2f delta, float dumping_rate) override;

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
    enum class State {
        Fluttering,
        Falling,
        Rising
    } state_ = State::Fluttering;
    SoundControllerT& sound_;
    AnimControllerT anim_;
    float dumping_rate_ = 0.5F;
    mgc::math::Vec2f force_ex_;
    mgc::math::Vec2f velocity_;
    BalloonAnimState anim_state_;
    BlinkAnimatorT blink_animator_;
    StopwatchT sw_;
    uint32_t update_bgm_param_count_ = 0;
    bool is_direction_right() const {
        return (
            ( anim_state_ == BalloonAnimState::FlutterRight ) ||
            ( anim_state_ == BalloonAnimState::FallRight )
        );
    }
    mgc_world_t min_x_;
    mgc_world_t max_x_;
    mgc::math::Vec2i orig_pos_;
    bool hit_wall_ = false;
    const int16_t VIEW_RANGE_X = 16;
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_BALLOON_HPP*/

