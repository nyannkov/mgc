#ifndef MGC_EVENT_WARP_HPP
#define MGC_EVENT_WARP_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/event/event.hpp"
#include "entity/prop/portal/portal.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

struct Warp : event::Event {

    explicit Warp(prop::Portal& portal);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override;

    void set_destination(SceneId scene_id) {
        dest_id_ = scene_id;
    }

private:
    prop::Portal& portal_;
    ScreenFader screen_fader_;
    SceneId dest_id_ = SceneId::Shop;
};

}// namespace app

#endif/*MGC_EVENT_WARP_HPP*/

