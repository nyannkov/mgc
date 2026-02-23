#ifndef MGC_CIVILIAN_HEN_HPP
#define MGC_CIVILIAN_HEN_HPP

#include "app_common.hpp"
#include "entity/civilian/civilian.hpp"

namespace app {
namespace civilian {

enum class HenAnimState {
    Stand_Left,
    Stand_Right,
//    Crouch_Left,
//    Crouch_Right,
//    LookUp_Left,
    LookUp_Right
};

struct Hen : Civilian {
    explicit Hen(const GamepadT& gamepad);
    ~Hen() = default;
    Hen(const Hen&) = delete;
    Hen& operator=(const Hen&) = delete;
    Hen(Hen&&) = default;
    Hen& operator=(Hen&&) = default;

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, HenAnimState anim_state);
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    void set_anim_state(HenAnimState anim_state);
    HenAnimState anim_state() const { return anim_state_; }

    void on_player_in_view(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

private:
    const GamepadT& gamepad_;
    HenAnimState anim_state_;
    bool talking_flag_ = false;
};

}// namespace civilian
}// namespace app

#endif/*MGC_CIVILIAN_HEN_HPP*/

