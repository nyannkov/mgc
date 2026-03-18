#ifndef MGC_TELEPORTER_HPP
#define MGC_TELEPORTER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_id.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

enum class TeleporterType {
    Type1,
};

struct Teleporter : Prop {
    explicit Teleporter(const GamepadT& gamepad);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, const mgc::math::Vec2i& dest);
    void despawn() override;
    void update_movement() override {};
    void update_animation() override {};

    void set_destination(const mgc::math::Vec2i& dest) {
        dest_ = dest;
    }
    auto destination() const { return dest_; }

    void set_type(TeleporterType type) {
        type_ = type;
    }
    auto type() const { return type_; }

    void reset_entry() { entry_flag_ = false; }
    bool try_entry() const { return entry_flag_; }

private:
    const GamepadT& gamepad_;
    bool entry_flag_ = false;
    TeleporterType type_ = TeleporterType::Type1;
    mgc::math::Vec2i dest_{};
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_TELEPORTER_HPP
