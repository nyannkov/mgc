#ifndef MGC_EFFECT_HPP
#define MGC_EFFECT_HPP

#include "app_common.hpp"

namespace app {

struct Effect : mgc::entities::ActorImpl<Effect, 1> {

    Effect(const FrameTimerT& frame_timer, const GamepadT& gamepad);
    ~Effect() = default;
    Effect(const Effect&) = delete;
    Effect& operator=(const Effect&) = delete;
    Effect(Effect&&) = default;
    Effect& operator=(Effect&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right);
    void despawn();
    bool is_right() const { return is_right_; }

private:
    const GamepadT& gamepad_;
    const FrameTimerT& frame_timer_;
    mgc::math::Vec2f velocity_;
    bool is_right_;
};


}// namespace app


#endif/*MGC_PLAYER_HPP*/

