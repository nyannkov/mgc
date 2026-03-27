#ifndef MGC_EVENTS_OBJECTS_SHOP_HPP
#define MGC_EVENTS_OBJECTS_SHOP_HPP

#include "event_shop_1.hpp"
#include "event_shop_2.hpp"
#include "scene/interface/ievent_objects.hpp"
#include "scene_objects/scene_objects_shop.hpp"
#include "entity/event/warp/warp.hpp"

namespace app {

struct EventObjects_Shop : IEventObjects {

    EventObjects_Shop(SceneContext& scx, SceneObjects_Shop& objs)
        : cp_info_(scx.world_state.checkpoint_info),
          prev_scene_id_(scx.world_state.scene_info.prev_scene_id()),
          event_1_(scx, objs),
          event_2_(scx, objs),
          warp_(objs.portal()),
          events_ { &event_1_, &event_2_, &warp_ } {

          init();
    }
    
    void init() override {
        event_1_.spawn();
        event_2_.spawn();
        warp_.spawn();
        warp_.set_destination(prev_scene_id_);
    }

    ArrayViewer<event::Event*> events() override {
        return { events_.data(), events_.size() };
    }

private:
    CheckpointInfo& cp_info_;
    Event_Shop1 event_1_;
    Event_Shop2 event_2_;
    Warp warp_;
    std::array<event::Event*, 3> events_;
    const SceneId prev_scene_id_;
};

}// namespace app

#endif/*MGC_EVENTS_OBJECTS_SHOP_HPP*/

