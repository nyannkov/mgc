#ifndef MGC_ENEMY_ROCKER_HPP
#define MGC_ENEMY_ROCKER_HPP

#include "app_common.hpp"
#include "entity/enemy/enemy.hpp"
#include "rocker_anim.hpp"

namespace app {
namespace enemy {

constexpr int32_t RockerMaxHP = 30;

enum class RockerState {
    Stopped,
    Playing,
    Singing,
    StepForward,
    StepBackward,
};

struct Rocker: Enemy {

    Rocker(
        const FrameTimerT& timer,
        SoundControllerT& sound
    );
    ~Rocker() = default;
    Rocker(const Rocker&) = delete;
    Rocker& operator=(const Rocker&) = delete;
    Rocker(Rocker&&) = default;
    Rocker& operator=(Rocker&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    void set_shadow_mode(bool onoff) { shadow_mode_ = onoff; }
    auto shadow_mode() const { return shadow_mode_; }

    auto rocker_state() const { return state_; }

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

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override;

private:
    SoundControllerT& sound_;
    AnimControllerT anim_;
    mgc::math::Vec2f force_ex_;
    mgc::math::Vec2f velocity_;
    RockerAnimState anim_state_;
    RockerState state_;
    uint32_t update_bgm_param_count_ = 0;
    BlinkAnimatorT blink_animator_;
    StopwatchT sw_;
    bool shadow_mode_ = false;
    mgc_color_t color_palette_[32];
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_ROCKER_HPP*/

