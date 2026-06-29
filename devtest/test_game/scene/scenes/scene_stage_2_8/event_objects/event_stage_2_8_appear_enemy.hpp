#ifndef MGC_EVENT_STAGE_2_8_APPEAR_ENEMY_HPP
#define MGC_EVENT_STAGE_2_8_APPEAR_ENEMY_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_stage_2_8.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

struct Event_Stage2_8_AppearEnemy : event::Event {

    Event_Stage2_8_AppearEnemy(
        SceneContext& scx,
        SceneObjects_Stage2_8& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override {}

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    SceneObjects_Stage2_8& objs_;
    StopwatchT sw_;
    Player& player_;
    enum class EventDetail {
        Idle,
        Ready,
        Steady,
        Fight,
        Victory
    } detail_ = EventDetail::Idle;
};

}// namespace app

#endif/*MGC_EVENT_STAGE_2_8_APPEAR_ENEMY_HPP*/

