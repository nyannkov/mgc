#ifndef MGC_ENEMY_SKY_FISH_HPP
#define MGC_ENEMY_SKY_FISH_HPP

#include "app_common.hpp"
#include "skyfish_anim.hpp"
#include "skyfish_bt.hpp"
#include "entity/enemy/enemy.hpp"
#include "game_context/player_stats.hpp"

namespace app {
namespace enemy {

constexpr int32_t SkyFishMaxHP = 3;

struct SkyFish: Enemy {

    SkyFish(
        const FrameTimerT& frame_timer,
        SoundControllerT& sound_controller,
        const Player& player,
        PlayerStats& player_stats
    );
    ~SkyFish() = default;
    SkyFish(const SkyFish&) = delete;
    SkyFish& operator=(const SkyFish&) = delete;
    SkyFish(SkyFish&&) = default;
    SkyFish& operator=(SkyFish&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void despawn() override;
    void pre_update() override;
    void post_update() override;

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
        const Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_collision_resolved(
        const CollisionTileLayer& layer,
        const mgc::collision::MapPushbackInfo& info
    ) override;

private:
    const Player& player_;
    SoundControllerT& sound_controller_;
    PlayerStats& player_stats_;
    AnimControllerT anim_;
    mgc::math::Vec2f velocity_;
    mgc::math::Vec2f force_ex_;
    SkyFishAnimState anim_state_;
    BTreeControllerT bt_;
    SkyFishBTListener bt_listener_;
    BlinkAnimatorT blink_animator_;
    bool is_direction_right() const {
        return (
            ( anim_state_ == SkyFishAnimState::HoverRight ) ||
            ( anim_state_ == SkyFishAnimState::RiseRight )  ||
            ( anim_state_ == SkyFishAnimState::FallRight )
        );
    }
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_SKY_FISH_HPP*/

