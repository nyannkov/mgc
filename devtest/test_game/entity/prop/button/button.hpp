#ifndef MGC_BUTTON_HPP
#define MGC_BUTTON_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

struct Button : Prop {
    Button();
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, bool pushed);
    void despawn() override;
    void update_movement() override {};
    void update_animation() override {};
    bool is_pushed() const { return pushed_; }

private:
    bool pushed_ = false;
    void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_BUTTON_HPP
