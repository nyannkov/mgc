#ifndef MGC_EVENT_TOWER_FRONT1_HPP
#define MGC_EVENT_TOWER_FRONT1_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "entity/player/player.hpp"
#include "scene_objects/scene_objects_tower_front.hpp"

namespace app {

struct Event_TowerFront1 : event::Event {
    
    enum class EventName {
        None,
        Point1,
        Point2,
        Point3,
        Point4,
        Point5,
        Point6,
        Point7,
        Point8,
    };

    enum class State {
        Scene0,
        Scene1,
        Scene2,
        Scene3,
        Scene4,
        Scene5,
        Scene6,
        Scene7,
        Scene8,
        Scene9,
        Scene10,
        Scene11,
        End,
    };

    Event_TowerFront1(
        SceneContext& scx,
        SceneObjects_TowerFront& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    void update() override;

private:
    SoundControllerT& sound_;
    StopwatchT sw_;
    CheckpointInfo& cp_info_;
    Player& player_;
    civilian::Hen& hen_;
    civilian::Chick& chick_;
    prop::Entrance& entrance_;
    State state_ = State::Scene0;

    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    struct TalkflowListener : ITalkflowListenerT {

        auto event() const { return event_name_; }

        void on_message_done(mgc_node_idx_t tag) override;

        void on_flow_end(mgc_node_idx_t tag) override { }

    private:
        EventName event_name_ = EventName::None;
    } talkflow_listener_;
};

}// namespace app

#endif/*MGC_EVENT_TOWER_FRONT1_HPP*/

