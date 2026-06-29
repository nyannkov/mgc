#include "resources/mml/mml.h"
#include "event_log_terminal_1.hpp"


namespace app {

using app::event::EventState;

Event_LogTerminal1::Event_LogTerminal1(
    SceneContext& scx,
    SceneObjects_LogTerminal& objs
) : sound_(scx.sound),
    sw_(scx.timer),
    cp_info_(scx.world_state.checkpoint_info),
    equip_info_(scx.world_state.equipment_info),
    signboard_(objs.signboard()),
    gate_(objs.gate()),
    jewel_(objs.jewel()),
    terminal_(objs.terminal()),
    player_(scx.player) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(6)});
    hitboxes[0].set_enabled(false);

    set_event_state(EventState::NotStarted);
}

void Event_LogTerminal1::spawn(const mgc::math::Vec2i& pos) {

    state_ = SceneState::NotStarted;

    this->set_position(pos);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    unlock_control();
    sw_.reset();

    if ( !equip_info_.item.has_item_at(ItemId::VioletJewel) ) {
        jewel_.spawn({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(10)+8});
        jewel_.hide();
    } else {
        jewel_.despawn();
    }
}

void Event_LogTerminal1::spawn() {
    spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(5)});
}

void Event_LogTerminal1::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_LogTerminal1::update() {
    
    if ( event_state() != EventState::Playing ) {
        return;
    }

    switch ( state_ ) {
    case SceneState::NotStarted:
        break;
    case SceneState::TurnOnTerminal:
        if ( player_.is_ground() ) {
            sw_.restart();
            state_ = SceneState::Wait_1_1;
            player_.set_anim_mode(PlayerAnimMode::Manual);
            player_.set_anim_manually(PlayerAnimState::LookupLeft);
            sound_.play_sound_effect(MML_SE_17_TURN_ON);
        }
        break;
    case SceneState::Wait_1_1:
        if ( sw_.elapsed_ms() > 1000 ) {
            state_ = SceneState::Wait_1_2;
            terminal_.turn_on();
            if ( !equip_info_.item.has_item_at(ItemId::VioletJewel) ) {
                jewel_.show();
            }
        }
        break;
    case SceneState::Wait_1_2:
        if ( sw_.elapsed_ms() > 2987 ) {
            set_and_trigger_talkflow_request({
                &talkscript_1,
                MGC_TALKSCRIPT_1_LOG_1,
                &talkflow_listener_,
                TalkflowEffectType::Mute
            });
            state_ = SceneState::ReadLog;
        }
        break;
    case SceneState::ReadLog:
        if ( talkflow_listener_.is_end() ) {
            state_ = SceneState::Wait_2_1;
            sw_.restart();
        }
        break;
    case SceneState::Wait_2_1:
        if ( sw_.elapsed_ms() > 1598 ) {
            state_ = SceneState::Wait_2_2;
            sw_.restart();
            terminal_.turn_off();
            if ( !equip_info_.item.has_item_at(ItemId::VioletJewel) ) {
                jewel_.hide();
            }
        }
        break;
    case SceneState::Wait_2_2:
        if ( sw_.elapsed_ms() > 1000 ) {
            if ( !equip_info_.item.has_item_at(ItemId::VioletJewel) ) {
                state_ = SceneState::SearchJewel;
            } else {
                state_ = SceneState::End;
            }
            unlock_control();
            player_.set_anim_mode(PlayerAnimMode::Auto);
        }
        break;

    case SceneState::SearchJewel:
        if ( jewel_.found() ) {
            set_and_trigger_talkflow_request({
                &talkscript_1,
                MGC_TALKSCRIPT_1_FOUND_JEWEL,
                &talkflow_listener_,
                TalkflowEffectType::Mute
            });
            equip_info_.item.add(ItemId::VioletJewel);
            state_ = SceneState::End;
        }
        break;

    case SceneState::End:
        set_event_state(EventState::Finished);
        break;
    default:
        break;
    }
}

void Event_LogTerminal1::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( event_state() == EventState::NotStarted ) {
        set_event_state(EventState::Playing);
        state_ = SceneState::TurnOnTerminal;
        lock_control();
    }
}


} // namespace app

