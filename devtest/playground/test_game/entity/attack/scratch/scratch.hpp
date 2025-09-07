#ifndef MGC_SCRATCH_HPP
#define MGC_SCRATCH_HPP

#include "entity/attack/attack.hpp"

namespace app {

struct Scratch : Attack {

    Scratch(const FrameTimerT& frame_timer, const GamepadT& gamepad);

    void spawn(const mgc::math::Vec2i& pos, AttackOwner owner, AttackDirection dir) override;
    void despawn() override;

private:
    const GamepadT& gamepad_;
    const FrameTimerT& frame_timer_;
};


}// namespace app

#endif/*MGC_SCRATCH_HPP*/

