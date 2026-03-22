#ifndef MGC_DOOR_HPP
#define MGC_DOOR_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_id.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

struct Door : Prop {
    explicit Door(const GamepadT& gamepad);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, bool locked);
    void despawn() override;
    void update_movement() override {};
    void update_animation() override {};

    void lock();
    void unlock();
    bool is_locked() const { return is_locked_; }

private:
    const GamepadT& gamepad_;
    bool is_locked_;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_DOOR_HPP
