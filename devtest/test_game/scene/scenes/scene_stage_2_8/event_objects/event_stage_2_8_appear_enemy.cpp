#include "resources/mml/mml.h"
#include "event_stage_2_8_appear_enemy.hpp"
#include "event_stage_2_8_common.hpp"

namespace app {

using app::event::EventState;

Event_Stage2_8_AppearEnemy::Event_Stage2_8_AppearEnemy(
    SceneContext& scx,
    SceneObjects_Stage2_8& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    objs_(objs),
    sw_(scx.timer),
    player_(scx.player) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_8_AppearEnemy::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
}

void Event_Stage2_8_AppearEnemy::spawn() {
    spawn({0, 0});
}

void Event_Stage2_8_AppearEnemy::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_8_AppearEnemy::update() {

    if ( event_state() != EventState::Playing ) return;

    switch (detail_) {
    case EventDetail::Idle:
        if ( player_.position().x > MGC_CELL2PIXEL(34) ) {
            detail_ = EventDetail::Ready;
            sound_.play_sound_effect(MML_SE_16_SPLAAAAAAASH);
            lock_control();
            sw_.restart();
            //objs_.lancer().spawn({MGC_CELL2PIXEL(37), MGC_CELL2PIXEL(11)}, true);
            objs_.lancer().spawn({MGC_CELL2PIXEL(37), MGC_CELL2PIXEL(13)}, true);
            objs_.lancer().set_enabled_hitbox_body(false);
        }
        break;

    case EventDetail::Ready: {
        
        auto pos = objs_.lancer().position();
        if ( pos.y > MGC_CELL2PIXEL(11) ) {
            pos.y -= 2;
        } else {
            if ( sound_.have_all_sound_effects_finished() ) {
                detail_ = EventDetail::Steady;
                sw_.restart();
            }
        }
        objs_.lancer().set_position(pos);
        break;
    }

    case EventDetail::Steady:
        
        if ( sw_.elapsed_ms() >= 1587 ) {
            sw_.restart();
            detail_ = EventDetail::Fight;
            sound_.play_background_music(MML_BGM_5_BOSS);
            objs_.lancer().set_enabled_hitbox_body(true);
            objs_.lancer().fight();
            unlock_control();
        }
        break;

    case EventDetail::Fight:
        if ( ( objs_.lancer().enemy_state() == enemy::EnemyState::Despawning ) ||
             ( objs_.lancer().enemy_state() == enemy::EnemyState::Inactive )
        ) {
            for ( auto& floaty : objs_.floaty() ) {
                if ( floaty.enemy_state() == enemy::EnemyState::Active ) {
                    floaty.despawn();
                }
            }
            sound_.stop_background_music();
        } else {
            auto water_level = objs_.stage().water_position().y;
            if ( water_level <= MAX_WATER_LEVEL ) {
                if ( sound_.last_bgm_param() >= 3 ) {
                   mgc::math::Vec2i pos = { MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(12) }; 
                    for ( auto& floaty : objs_.floaty() ) {
                        pos.x += MGC_CELL2PIXEL(9); 
                        if ( floaty.enemy_state() == enemy::EnemyState::Inactive ) {
                            floaty.spawn(pos, false);
                        }
                    }
                }
            }
        }

        if ( player_.is_game_over() ) {
            //objs_.lancer().wait();
        }

        break;
    case EventDetail::Victory:
        cp_info_.advance_checkpoint(Checkpoint::WonTheStage2Boss);
        break;
    default:
        break;
    }
}

} // namespace app

