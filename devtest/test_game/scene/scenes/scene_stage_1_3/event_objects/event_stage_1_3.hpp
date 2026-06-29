#ifndef MGC_EVENT_STAGE_1_3_HPP
#define MGC_EVENT_STAGE_1_3_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_stage_1_3.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

struct Event_Stage1_3 : event::Event {

    Event_Stage1_3(
        SceneContext& scx,
        SceneObjects_Stage1_3& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {}

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    prop::Gate& locked_gate_;
    prop::Button& button_;
};

}// namespace app

#endif/*MGC_EVENT_STAGE_1_3_HPP*/

