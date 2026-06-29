#ifndef MGC_EVENT_STAGE_2_5_HPP
#define MGC_EVENT_STAGE_2_5_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_stage_2_5.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

struct Event_Stage2_5 : event::Event {

    Event_Stage2_5(
        SceneContext& scx,
        SceneObjects_Stage2_5& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override;

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    std::array<prop::Button, 5>& buttons_;
    LabelT& label_counter_;
    int32_t counter_ = 0;
    char counter_buffer_[5];
    bool wait_flag_ = false;
};

}// namespace app

#endif/*MGC_EVENT_STAGE_2_5_HPP*/

