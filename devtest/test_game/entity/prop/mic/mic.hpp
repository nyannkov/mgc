#ifndef MGC_MIC_HPP
#define MGC_MIC_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

struct Mic : Prop {
    explicit Mic(const GamepadT& gamepad);
    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    void update_movement() override {}
    void update_animation() override;

private:
    const GamepadT& gamepad_;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_MIC_HPP
