#include "event_tower_front_1.hpp"
#include "scene/scene_id.hpp"
#include "scene_objects/scene_objects_tower_front.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

using app::civilian::HenAnimState;
using app::civilian::ChickAnimState;
using app::event::EventState;
using EventName = app::Event_TowerFront1::EventName;

Event_TowerFront1::Event_TowerFront1(
    SceneContext& scx,
    SceneObjects_TowerFront& objs
) : sound_(scx.sound),
    sw_(scx.timer),
    cp_info_(scx.world_state.checkpoint_info),
    player_(scx.player),
    hen_(objs.hen()),
    chick_(objs.chick()),
    entrance_(objs.entrance()) { 

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({32, 300});
    hitboxes[0].set_enabled(false);

    set_event_state(EventState::NotStarted);
}


void Event_TowerFront1::spawn(const mgc::math::Vec2i& pos) {
    this->set_position(pos);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    unlock_control();
}

void Event_TowerFront1::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_TowerFront1::update() {
    
    if ( event_state() != EventState::Playing ) {
        return;
    }

    auto event = talkflow_listener_.event();
    switch (state_) {
    case Event_TowerFront1::State::Scene0:
        break;
    case Event_TowerFront1::State::Scene1:
        if ( player_.input_enabled() ) {
            player_.set_input_enabled(false);
        }
        if ( player_.is_ground() ) {
            set_and_trigger_talkflow_request({
                &talkscript_1,
                MGC_TALKSCRIPT_1_TOWER_FRONT_TALK_1,
                &talkflow_listener_,
                TalkflowEffectType::Default
            });

            state_ = Event_TowerFront1::State::Scene2;
            player_.set_anim_mode(PlayerAnimMode::Manual);
            player_.set_anim_manually(PlayerAnimState::StandRight);

            hen_.set_anim_state(HenAnimState::Stand_Left);
        }
        break;
    case Event_TowerFront1::State::Scene2:
        if ( event == EventName::Point1 ) {
            state_ = Event_TowerFront1::State::Scene3;

            hen_.set_anim_state(HenAnimState::LookUp_Right);
            player_.set_anim_manually(PlayerAnimState::LookupRight);
        }
        break;
    case Event_TowerFront1::State::Scene3:
        if ( event == EventName::Point2 ) {
            state_ = Event_TowerFront1::State::Scene4;

            hen_.set_anim_state(HenAnimState::Stand_Left);
        }
        break;
    case Event_TowerFront1::State::Scene4:
        if ( event == EventName::Point3 ) {
            state_ = Event_TowerFront1::State::Scene5;

            hen_.set_anim_state(HenAnimState::Stand_Left);
            chick_.set_anim_state(ChickAnimState::Stand_Left);
            player_.set_anim_manually(PlayerAnimState::StandRight);
        }
        break;
    case Event_TowerFront1::State::Scene5:
        if ( event == EventName::Point4 ) {
            state_ = Event_TowerFront1::State::Scene6;

            chick_.set_anim_state(ChickAnimState::Stand_Right);
            hen_.set_anim_state(HenAnimState::Stand_Right);
        }
        break;
    case Event_TowerFront1::State::Scene6:
        if ( event == EventName::Point5 ) {
            state_ = Event_TowerFront1::State::Scene7;
            sw_.restart();
        }
        break;

    case Event_TowerFront1::State::Scene7:
        if ( sw_.elapsed_ms() >= 1987 ) {
            entrance_.set_visible(true);
            state_ = Event_TowerFront1::State::Scene8;
            sound_.play_sound_effect(MML_SE_10_OPEN_2, 0.0);
            sw_.restart();
        }
        break;
    case Event_TowerFront1::State::Scene8:
        if ( sw_.elapsed_ms() >= 2700 ) {
            state_ = Event_TowerFront1::State::Scene9;
            set_and_trigger_talkflow_request({
                &talkscript_1,
                MGC_TALKSCRIPT_1_TOWER_FRONT_TALK_2,
                &talkflow_listener_,
                TalkflowEffectType::Default
            });
        }
        break;

    case Event_TowerFront1::State::Scene9:
        if ( event == EventName::Point6 ) {
            state_ = Event_TowerFront1::State::Scene10;
            hen_.set_anim_state(HenAnimState::Stand_Left);
        }
        break;

    case Event_TowerFront1::State::Scene10:
        if ( event == EventName::Point7 ) {
            state_ = Event_TowerFront1::State::End;
            if ( !player_.input_enabled() ) {
                player_.set_input_enabled(true);
                player_.set_anim_mode(PlayerAnimMode::Auto);
            }
            set_event_state(EventState::Finished);
            cp_info_.advance_checkpoint(Checkpoint::TowerFrontEvent_Finished);

            hen_.set_talkflow_request({
                &talkscript_1,
                MGC_TALKSCRIPT_1_TOWER_FRONT_TALK_3,
                nullptr,
                TalkflowEffectType::Default
            });
        }
        break;

    case Event_TowerFront1::State::End:
        break;
    default:
        break;
    }
}

void Event_TowerFront1::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( event_state() == EventState::NotStarted ) {
        set_event_state(EventState::Playing);
        state_ = Event_TowerFront1::State::Scene1;
    }
}

void Event_TowerFront1::TalkflowListener::on_message_done(mgc_node_idx_t tag) {
    switch (tag) {
    case MGC_TALKSCRIPT_1_TAG_MESSAGE__POINT_1:
        event_name_ = EventName::Point1;
        break;
    case MGC_TALKSCRIPT_1_TAG_MESSAGE__POINT_2:
        event_name_ = EventName::Point2;
        break;
    case MGC_TALKSCRIPT_1_TAG_MESSAGE__POINT_3:
        event_name_ = EventName::Point3;
        break;
    case MGC_TALKSCRIPT_1_TAG_MESSAGE__POINT_4:
        event_name_ = EventName::Point4;
        break;
    case MGC_TALKSCRIPT_1_TAG_MESSAGE__POINT_5:
        event_name_ = EventName::Point5;
        break;
    case MGC_TALKSCRIPT_1_TAG_MESSAGE__POINT_6:
        event_name_ = EventName::Point6;
        break;
    case MGC_TALKSCRIPT_1_TAG_MESSAGE__POINT_7:
        event_name_ = EventName::Point7;
        break;
    default:
        event_name_ = EventName::None;
        break;
    }
}



}// namespace app

