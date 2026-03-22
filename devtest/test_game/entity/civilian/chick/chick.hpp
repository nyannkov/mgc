#ifndef MGC_CIVILIAN_CHICK_HPP
#define MGC_CIVILIAN_CHICK_HPP

#include "app_common.hpp"
#include "entity/civilian/civilian.hpp"

namespace app {
namespace civilian {

enum class ChickAnimState {
    Stand_Left,
    Stand_Right,
//    Crouch_Left,
//    Crouch_Right,
//    LookUp_Left,
    LookUp_Right
};

struct Chick : Civilian {
    explicit Chick(const GamepadT& gamepad);
    ~Chick() = default;
    Chick(const Chick&) = delete;
    Chick& operator=(const Chick&) = delete;
    Chick(Chick&&) = default;
    Chick& operator=(Chick&&) = default;

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, ChickAnimState anim_state);
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    void set_anim_state(ChickAnimState anim_state);
    ChickAnimState anim_state() const { return anim_state_; }

private:
    const GamepadT& gamepad_;
    ChickAnimState anim_state_;
    bool talking_flag_ = false;
};

}// namespace civilian
}// namespace app

#endif/*MGC_CIVILIAN_CHICK_HPP*/

