#ifndef MGC_ENEMY_SKY_FISH_HPP
#define MGC_ENEMY_SKY_FISH_HPP

#include "app_common.hpp"
#include "skyfish_anim.hpp"
#include "skyfish_hitbox.hpp"
#include "skyfish_bt.hpp"

namespace app {

struct Player;

namespace enemy {

struct SkyFish : mgc::entities::ActorImpl<SkyFish, static_cast<size_t>(SkyFishHitboxIndex::Count)> {

    explicit SkyFish(const FrameTimerT& frame_timer, const Player& player);
    ~SkyFish() = default;
    SkyFish(const SkyFish&) = delete;
    SkyFish& operator=(const SkyFish&) = delete;
    SkyFish(SkyFish&&) = default;
    SkyFish& operator=(SkyFish&&) = default;

    void set_spawn_point(const mgc::math::Vec2i& pos, SkyFishAnimState anim_state);
    void prepare_update();
    void finalize_update();

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, Player>) {
            bt_listener_.set_hit_flag(info.self_hitbox_index);
        }
    }

private:
    const Player& player_;
    AnimControllerT anim_;
    mgc::math::Vec2f velocity_;
    SkyFishAnimState anim_state_;
    BTreeControllerT bt_;
    SkyFishBTListener bt_listener_;
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

