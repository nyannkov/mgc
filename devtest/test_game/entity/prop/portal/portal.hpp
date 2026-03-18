#ifndef MGC_PORTAL_HPP
#define MGC_PORTAL_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_id.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

struct Portal : Prop {
    explicit Portal(const GamepadT& gamepad, const FrameTimerT& frame_timer);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, bool locked);
    void despawn() override;
    void update_movement() override {};
    void update_animation() override;

    bool try_entry() const { return try_entry_; }
    void request_scene_change(SceneTransitionRequest request) {
        set_and_trigger_scene_transition_request(request);
    }

private:
    const GamepadT& gamepad_;
    AnimControllerT anim_;
    bool try_entry_ = false;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_PORTAL_HPP
