#ifndef MGC_EVENT_STAGE_3_5_HPP
#define MGC_EVENT_STAGE_3_5_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_stage_3_5.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

struct Event_Stage3_5 : event::Event {

    Event_Stage3_5(
        SceneContext& scx,
        SceneObjects_Stage3_5& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {}

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    prop::Button& button_;
    carrier::WaypointCarrier<1, 3>& carrier_;

    std::array<carrier::WaypointT, 3> wp_button_off_ = {{
        {MGC_CELL2PIXEL(54), MGC_CELL2PIXEL(25)},
        {MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(25)},
        {MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(15)},
    }};

    std::array<carrier::WaypointT, 4> wp_button_on_ = {{
        {MGC_CELL2PIXEL(54), MGC_CELL2PIXEL(25)},
        {MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(25)},
        {MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(15)},
        {MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(15)},
    }};

};

}// namespace app

#endif/*MGC_EVENT_STAGE_3_5_HPP*/

