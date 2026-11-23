#ifndef MGC_GLOW_GATE_HPP
#define MGC_GLOW_GATE_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_id.hpp"
#include "world_state/equipment_info.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

struct GlowGate : Prop {
    explicit GlowGate(const GamepadT& gamepad);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, bool open);
    void despawn() override;
    void update_movement() override {}
    void update_animation() override;

    void open();
    void close();
    bool is_opened() const { return is_opened_; }

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override;

private:
    const GamepadT& gamepad_;
    bool is_opened_;
    size_t get_tile_idx(bool is_open) const;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
    int16_t alpha_ = 0;
    bool hit_flag_ = false;
};

} // namespace prop
} // namespace app

#endif // MGC_GLOW_GATE_HPP
