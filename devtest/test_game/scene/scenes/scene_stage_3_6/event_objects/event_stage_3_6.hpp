#ifndef MGC_EVENT_STAGE_3_6_HPP
#define MGC_EVENT_STAGE_3_6_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "entity/event/visit/visit.hpp"
#include "scene_objects/scene_objects_stage_3_6.hpp"

namespace app {

using VisitArrayT = std::array<Visit, 7>;

struct Event_Stage3_6 : event::Event {

    Event_Stage3_6(
        SceneContext& scx,
        SceneObjects_Stage3_6& objs,
        CameraT& camera,
        VisitArrayT& visit
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override;

private:
    enum class EventDetail {
        Idle,
        Lookup,
        Appear,
        Cheer,
        Fight,
        Cheer2,
        Fadeout,
        Exit,
        Fadein,
        Victory
    };

    void invite_walker(mgc::math::Vec2i pos, Visit& visit, enemy::Walker& walker) {
        
        if ( ( visit.event_state() != event::EventState::Playing ) &&
             ( walker.enemy_state() == enemy::EnemyState::Inactive )
        ) {
            visit.set_draw_behind_mode(true);
            walker.set_spawn_animation_enabled(true);
            walker.set_walker_mode(enemy::WalkerMode::Dancing);
            visit.set_visitor(&walker);
            visit.spawn(pos);
        }
    }

    void invite_skyfish(mgc::math::Vec2i pos, Visit& visit, enemy::SkyFish& skyfish) {
        if ( ( visit.event_state() != event::EventState::Playing ) &&
             ( skyfish.enemy_state() == enemy::EnemyState::Inactive )
        ) {
            visit.set_draw_behind_mode(true);
            visit.set_visitor(&skyfish);
            visit.spawn(pos);
        }
    }

    void invite_balloon(mgc::math::Vec2i pos, Visit& visit, enemy::Balloon& balloon) {
        if ( ( visit.event_state() != event::EventState::Playing ) &&
             ( balloon.enemy_state() == enemy::EnemyState::Inactive )
        ) {
            visit.set_draw_behind_mode(true);
            visit.set_visitor(&balloon);
            visit.spawn(pos);
        }
    }

    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    Player& player_;
    StopwatchT sw_;
    SceneObjects_Stage3_6& objs_;
    EventDetail detail_ = EventDetail::Idle;
    CameraT& camera_;
    VisitArrayT&  visit_;
    int n_popped_target_balloon_;
    int n_created_balloon_;
    bool finish_flag_;
    const int MAX_BALLOON_NUM_ = 10;
    ScreenFader screen_fader_;
};

}// namespace app

#endif/*MGC_EVENT_STAGE_3_6_HPP*/

