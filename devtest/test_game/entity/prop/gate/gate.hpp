#ifndef MGC_GATE_HPP
#define MGC_GATE_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_id.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

enum class GateType {
    Type1,
    Type2,
};

struct Gate : Prop {
    explicit Gate(const GamepadT& gamepad);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, bool open);
    void despawn() override;
    void update_movement() override {};
    void update_animation() override {};

    void open();
    void close();
    bool is_opened() const { return is_opened_; }

    auto gate_type() const { return gate_type_; }
    void set_gate_type(GateType type);

private:
    const GamepadT& gamepad_;
    bool is_opened_;
    GateType gate_type_;
    size_t get_tile_idx(GateType gate_type, bool is_open) const;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_GATE_HPP
