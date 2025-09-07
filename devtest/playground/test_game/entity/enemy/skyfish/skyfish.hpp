#ifndef MGC_ENEMY_SKY_FISH_HPP
#define MGC_ENEMY_SKY_FISH_HPP

#include "app_common.hpp"
#include "skyfish_anim.hpp"
#include "skyfish_hitbox.hpp"
#include "skyfish_bt.hpp"
#include "entity/enemy/enemy.hpp"

namespace app {
namespace enemy {

constexpr int32_t SKYFISH_MAX_HP = 3;

struct SkyFish: Enemy {

    SkyFish(const FrameTimerT& frame_timer, const Player& player);
    ~SkyFish() = default;
    SkyFish(const SkyFish&) = delete;
    SkyFish& operator=(const SkyFish&) = delete;
    SkyFish(SkyFish&&) = default;
    SkyFish& operator=(SkyFish&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void despawn() override;
    void pre_update() override;
    void post_update() override;

    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_attack_hit(
        const Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

private:
    const Player& player_;
    AnimControllerT anim_;
    mgc::math::Vec2f velocity_;
    SkyFishAnimState anim_state_;
    BTreeControllerT bt_;
    SkyFishBTListener bt_listener_;
    mgc::math::Vec2f force_ex_;
    BlinkAnimator blink_animator_;

    void set_force_ex(mgc::math::Vec2f v) { force_ex_ = v; }
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

