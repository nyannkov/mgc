#ifndef MGC_EVENT_STAGE_2_7_HPP
#define MGC_EVENT_STAGE_2_7_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_stage_2_7.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

struct Event_Stage2_7 : event::Event {

    Event_Stage2_7(
        SceneContext& scx,
        SceneObjects_Stage2_7& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {}

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    SceneObjects_Stage2_7& objs_;
    bool wait_flag_ = false;
};

}// namespace app

#endif/*MGC_EVENT_STAGE_2_7_HPP*/

