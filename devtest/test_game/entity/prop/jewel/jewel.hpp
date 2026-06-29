#ifndef MGC_JEWEL_HPP
#define MGC_JEWEL_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

struct Jewel : Prop {
    explicit Jewel(const GamepadT& gamepad, const FrameTimerT& frame_timer);
    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    void update_movement() override {}
    void update_animation() override;
    bool found() const { return found_; }

    void hide() { this->mut_sprite().set_visible(false); }
    void show() { this->mut_sprite().set_visible(true); }

private:
    const GamepadT& gamepad_;
    AnimControllerT anim_;
    bool found_ = false;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_JEWEL_HPP
