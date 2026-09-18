#ifndef MGC_ENEMY_FLOATY_FISH_HPP
#define MGC_ENEMY_FLOATY_FISH_HPP

#include "app_common.hpp"
#include "entity/enemy/enemy.hpp"
#include "floaty_fish_anim.hpp"

namespace app {
namespace enemy {

constexpr int32_t FloatyFishMaxHP = 2;

struct FloatyFish: Enemy {
    FloatyFish(
        const FrameTimerT& timer,
        SoundControllerT& sound,
        const mgc::features::HasPosition<mgc::math::Vec2i>& player_pos
    );
    ~FloatyFish() = default;
    FloatyFish(const FloatyFish&) = delete;
    FloatyFish& operator=(const FloatyFish&) = delete;
    FloatyFish(FloatyFish&&) = default;
    FloatyFish& operator=(FloatyFish&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void spawn(const mgc::math::Vec2i& pos);
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

    void on_collision_resolved(
        const stage::LayerWater& water,
        const mgc::collision::MapPushbackInfo& info
    ) override;

private:
    SoundControllerT& sound_;
    AnimControllerT anim_;
    float dumping_rate_ = 0.5f;
    mgc::math::Vec2f force_ex_;
    mgc::math::Vec2f velocity_;
    FloatyFishAnimState anim_state_;
    BlinkAnimatorT blink_animator_;
    StopwatchT sw_;
    const mgc::features::HasPosition<mgc::math::Vec2i>& player_pos_;

    bool hit_water_ = false;
    bool hit_head_water_ = false;

    bool is_direction_right() const {
        return ( anim_state_ == FloatyFishAnimState::FloatingRight );
    }
    void update_movement_normal();
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_FLOATY_FISH_HPP*/

