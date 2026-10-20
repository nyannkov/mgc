#ifndef MGC_EVENT_VISIT_HPP
#define MGC_EVENT_VISIT_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/event/event.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

enum class VisitState {
    Stopped,
    FadeIn,
    Spawning,
    FadeOut,
    End
};

struct Visit : event::Event {

    explicit Visit(const FrameTimerT& timer);
    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override;
    void set_visitor(enemy::Enemy *visitor) { visitor_ = visitor; }
    auto *visitor() const { return visitor_; }

private:
    enemy::Enemy *visitor_ = nullptr;
    AnimControllerT anim_vortex_;
    StopwatchT sw_;
    VisitState visit_state_ = VisitState::Stopped;
};

}// namespace app

#endif/*MGC_EVENT_VISIT_HPP*/

