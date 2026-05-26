#ifndef MGC_EVENT_TELEPORT_H
#define MGC_EVENT_TELEPORT_H

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/event/event.hpp"
#include "utils/screen_fader/screen_fader.hpp"
#include "entity/prop/teleporter/teleporter.hpp"

namespace app {
namespace event {

struct Teleport : event::Event {

    Teleport(
        prop::Teleporter* teleporters, 
        size_t teleporters_count,
        Player& player
    );
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override;

private:
    prop::Teleporter* teleporters_;
    const size_t teleporters_count_;
    ScreenFader screen_fader_;
    Player& player_;
    prop::Teleporter* active_teleporter_ = nullptr;
};

}// namepsace event
}// namespace app

#endif/*MGC_EVENT_TELEPORT_H*/

