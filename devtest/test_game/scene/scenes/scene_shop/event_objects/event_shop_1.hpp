#ifndef MGC_EVENT_SHOP_1_HPP
#define MGC_EVENT_SHOP_1_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_shop.hpp"

namespace app {

struct Event_Shop1 : event::Event {

    Event_Shop1(
        SceneContext& scx,
        SceneObjects_Shop& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    StopwatchT sw_;
};

}// namespace app

#endif/*MGC_EVENT_SHOP_1_HPP*/

