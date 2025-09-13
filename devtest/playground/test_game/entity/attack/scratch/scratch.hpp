#ifndef MGC_SCRATCH_HPP
#define MGC_SCRATCH_HPP

#include "entity/attack/attack.hpp"

namespace app {

struct Scratch : Attack {

    Scratch(const FrameTimerT& frame_timer, const GamepadT& gamepad);

    void spawn(const mgc::math::Vec2i& pos, AttackOwner owner, AttackDirection dir) override;
    void despawn() override;

    int32_t apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const override;
    int32_t apply_damage_to(Player& player, size_t attack_hitbox_index) const override;

private:
    const GamepadT& gamepad_;
    const FrameTimerT& frame_timer_;
};


}// namespace app

#endif/*MGC_SCRATCH_HPP*/

