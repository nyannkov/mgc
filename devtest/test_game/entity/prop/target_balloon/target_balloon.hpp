#ifndef MGC_TARGET_BALLOON_HPP
#define MGC_TARGET_BALLOON_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

struct TargetBalloon : Prop {
    enum class State {
        Inactive,
        Active,
        Despawning,
        Despawning2,
        Despawned
    };

    explicit TargetBalloon(
        const FrameTimerT& frame_timer
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    auto state() const { return state_; }
    void reset_despawned_state() {
        if ( state_ == State::Despawned ) {
            state_ = State::Inactive;
        }
    }
    void set_goal(const mgc::math::Vec2i& pos) {
        goal_ = pos;
    }
    void set_gain(const mgc::math::Vec2f& gain) {
        gain_ = gain;
    }

    void set_balloon_color(ColorT color) {
        color_ = color;
        color_palette_[2] = MGC_COLOR_SWAP(color_);
    }

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override;

private:
    AnimControllerT anim_;
    StopwatchT sw_;
    State state_;
    mgc::math::Vec2i goal_;
    mgc::math::Vec2f velocity_;
    mgc::math::Vec2f gain_;
    ColorT color_;
    mgc_color_t color_palette_[6];

    void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace block
} // namespace app

#endif // MGC_TARGET_BALLOON_HPP
