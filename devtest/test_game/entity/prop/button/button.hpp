#ifndef MGC_BUTTON_HPP
#define MGC_BUTTON_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

enum class ButtonType {
    Type1,// Right
    Type2,// Up
    Type3,// Down
};

struct Button : Prop {
    explicit Button(ButtonType type = ButtonType::Type1);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, bool pushed);
    void despawn() override;
    void update_movement() override {};
    void update_animation() override {};
    bool is_pushed() const { return pushed_; }
    void set_pushed_state(bool pushed);
    void toggle_state();
    bool just_pushed() {
        bool r = just_pushed_;
        just_pushed_ = false;
        return r;
    }

private:
    bool pushed_ = false;
    bool just_pushed_ = false;
    ButtonType type_ = ButtonType::Type1;
    void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
    void update_sprite();
};

} // namespace prop
} // namespace app

#endif // MGC_BUTTON_HPP
