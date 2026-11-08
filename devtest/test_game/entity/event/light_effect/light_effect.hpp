#ifndef MGC_EVENT_LIGHT_HPP
#define MGC_EVENT_LIGHT_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/event/event.hpp"
#include "entity/prop/portal/portal.hpp"
#include "utils/screen_fader/screen_fader.hpp"
#include "utils/light/light.hpp"

namespace app {
namespace event {

struct LightEffect : event::Event {

    explicit LightEffect(
        const mgc::features::HasPosition<mgc::math::Vec2i>& player_pos
    );
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override;
    void set_wide_mode(bool m);
    auto wide_mode() const { return wide_mode_; }
    void set_offset(mgc::math::Vec2i ofs) { offset_ = ofs; }
    auto offset() const { return offset_; }

private:
    Light light_;
    bool wide_mode_;
    mgc::math::Vec2i offset_;
    const mgc::features::HasPosition<mgc::math::Vec2i>& player_pos_;
};

}// namespace event
}// namespace app

#endif/*MGC_EVENT_LIGHT_HPP*/

