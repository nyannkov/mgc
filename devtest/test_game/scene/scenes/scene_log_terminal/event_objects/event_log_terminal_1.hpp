#ifndef MGC_EVENT_LOG_TERMINAL_1_HPP
#define MGC_EVENT_LOG_TERMINAL_1_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "scene_objects/scene_objects_log_terminal.hpp"

namespace app {

struct Event_LogTerminal1 : event::Event {
    
    enum class SceneState {
        NotStarted,
        TurnOnTerminal,
        Wait_1_1,
        Wait_1_2,
        ReadLog,
        Wait_2_1,
        Wait_2_2,
        SearchJewel,
        End
    };

    Event_LogTerminal1(
        SceneContext& scx,
        SceneObjects_LogTerminal& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;

private:
    SoundControllerT& sound_;
    StopwatchT sw_;
    CheckpointInfo& cp_info_;
    EquipmentInfo& equip_info_;
    prop::Gate& gate_;
    prop::Signboard& signboard_;
    prop::Jewel& jewel_;
    Player& player_;
    Terminal& terminal_;
    ItemId jewel_id_;
    mgc_node_idx_t text_log_idx_;
    mgc_node_idx_t text_found_jewel_idx_;

    SceneState state_ = SceneState::NotStarted;

    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    struct TalkflowListener : ITalkflowListenerT {
        void on_flow_end(mgc_node_idx_t tag) override { 
            is_end_ = true;
        }
        bool is_end() const { return is_end_; }
    private:
        bool is_end_ = false;
    } talkflow_listener_;
};

}// namespace app

#endif/*MGC_EVENT_LOG_TERMINAL_1_HPP*/

