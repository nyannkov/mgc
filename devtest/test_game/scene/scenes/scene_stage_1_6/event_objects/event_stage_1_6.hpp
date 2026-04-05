#ifndef MGC_EVENT_STAGE_1_6_HPP
#define MGC_EVENT_STAGE_1_6_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_stage_1_6.hpp"

namespace app {

struct Event_Stage1_6 : event::Event {

    Event_Stage1_6(
        SceneContext& scx,
        SceneObjects_Stage1_6& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;

private:
    enum class EventDetail {
        Idle,
        Lookup,
        FallingDancer,
        Fight,
        Victory
    };

    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    Player& player_;
    StopwatchT sw_;
    SceneObjects_Stage1_6& objs_;
    EventDetail detail_ = EventDetail::Idle;
    stage::Stage* stage_;
};

}// namespace app

#endif/*MGC_EVENT_STAGE_1_6_HPP*/

