#include "resources/mml/mml.h"
#include "event_stage_1_6.hpp"

namespace app {

using app::event::EventState;


Event_Stage1_6::Event_Stage1_6(
    SceneContext& scx,
    SceneObjects_Stage1_6& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    player_(scx.player),
    sw_(scx.timer),
    objs_(objs) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage1_6::spawn(const mgc::math::Vec2i& pos) {
    
    objs_.stage().set_ladder_enabled(false);
    set_event_state(EventState::Playing);
    unlock_control();
}

void Event_Stage1_6::spawn() {
    spawn({0, 0});
}

void Event_Stage1_6::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage1_6::update() {

    if ( event_state() != EventState::Playing ) return;

    switch (detail_) {
    case EventDetail::Idle:
        if ( player_.position().x > MGC_CELL2PIXEL(26) ) {
            detail_ = EventDetail::Lookup;
            sound_.play_sound_effect(MML_SE_8_OPEN);
            lock_control();
            player_.set_anim_mode(PlayerAnimMode::Manual);
            player_.set_anim_manually(PlayerAnimState::LookupRight, false);
            sw_.restart();
        }
        break;

    case EventDetail::Lookup:
        if ( sw_.elapsed_ms() >= 1987 ) {
            sw_.restart();
            objs_.dancer().spawn({MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(10)}, true);
            detail_ = EventDetail::FallingDancer;
        }
        break;

    case EventDetail::FallingDancer:
        
        if ( objs_.dancer().position().y >= MGC_CELL2PIXEL(21) ) {
            player_.set_anim_manually(PlayerAnimState::StandRight, false);
        }
        if ( sw_.elapsed_ms() >= 1987 ) {
            sound_.play_background_music(MML_BGM_5_BOSS);
            unlock_control();
            player_.set_anim_mode(PlayerAnimMode::Auto);
            detail_ = EventDetail::Fight;
        }
        break;
    case EventDetail::Fight:
        
        if ( ( objs_.dancer().enemy_state() == enemy::EnemyState::Despawning ) ||
             ( objs_.dancer().enemy_state() == enemy::EnemyState::Inactive )
        ) {
            for ( auto& walker : objs_.walker() ) {
                if ( walker.enemy_state() == enemy::EnemyState::Active ) {
                    walker.despawn();
                }
            }
            sound_.stop_background_music();
            detail_ = EventDetail::Victory;
        } else {
            if ( sound_.last_bgm_param() >= 3 ) {
               mgc::math::Vec2i pos = { MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(10) }; 
                for ( auto& walker : objs_.walker() ) {
                    pos.x += MGC_CELL2PIXEL(3); 
                    if ( walker.enemy_state() == enemy::EnemyState::Inactive ) {
                        walker.spawn(pos, true, enemy::WalkerMode::Dancing);
                    }
                }
            }
        }
        break;
    case EventDetail::Victory:
        cp_info_.advance_checkpoint(Checkpoint::WonTheStage1Boss);
        objs_.stage().set_ladder_enabled(true);
        break;
    default:
        break;
    }
}

} // namespace app

