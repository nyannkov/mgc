#ifndef MGC_EVENT_CORRIDOR_1_HPP
#define MGC_EVENT_CORRIDOR_1_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_002_objects.hpp"

namespace app {

struct Event_Corridor1 : event::Event {

    Event_Corridor1(
        SceneContext& scx,
        Scene002_Objects& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    prop::Gate& gate_;
    prop::Signboard& signboard_;

    struct TalkflowListener : ITalkflowListenerT {
        uint32_t checked_count() const { return checked_count_; }
        void clear_checked_count() { checked_count_ = 0; }
        void on_flow_end(mgc_node_idx_t tag) override { 
            if ( checked_count_ < 100000 ) {
                checked_count_++;
            }
        }
    private:
        uint32_t checked_count_ = 0;
    } talkflow_listener_;
};

}// namespace app

#endif/*MGC_EVENT_CORRIDOR_1_HPP*/

