#include "resources/mml/mml.h"
#include "event_stage_3_6.hpp"

namespace app {

using app::event::EventState;


Event_Stage3_6::Event_Stage3_6(
    SceneContext& scx,
    SceneObjects_Stage3_6& objs,
    CameraT& camera,
    VisitArrayT& visit
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    player_(scx.player),
    sw_(scx.timer),
    objs_(objs),
    camera_(camera),
    visit_(visit) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage3_6::spawn(const mgc::math::Vec2i& pos) {

    screen_fader_.clear();
    screen_fader_.set_fade_speed(5);
    screen_fader_.set_fill_color(MGC_COLOR_WHITE);
    
    n_popped_target_balloon_ = 0;
    n_created_balloon_ = 0;
    finish_flag_ = false;
    
    objs_.stage().set_ladder_enabled(true);
    objs_.spotlight1().turn_off();
    objs_.spotlight2().turn_off();
    objs_.rocker().spawn({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(11)}, true);
    objs_.rocker().set_shadow_mode(true);

    // bottom left1
    objs_.spectator()[0].spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(16)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type1);
    objs_.spectator()[0].set_jump_check_interval(300);
    objs_.spectator()[1].spawn({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(16)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[1].set_jump_check_interval(410);
    objs_.spectator()[2].spawn({MGC_CELL2PIXEL(8)+8, MGC_CELL2PIXEL(16)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type3);
    objs_.spectator()[2].set_jump_check_interval(520);

    // bottom left1
    objs_.spectator()[3].spawn({MGC_CELL2PIXEL(6)+8, MGC_CELL2PIXEL(15)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[3].set_jump_check_interval(290);
    objs_.spectator()[4].spawn({MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(15)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type3);
    objs_.spectator()[4].set_jump_check_interval(320);
    objs_.spectator()[5].spawn({MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(15)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[5].set_jump_check_interval(450);

    // top left1
    objs_.spectator()[6].spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(7)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type1);
    objs_.spectator()[6].set_jump_check_interval(300);
    objs_.spectator()[7].spawn({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(7)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[7].set_jump_check_interval(410);
    objs_.spectator()[8].spawn({MGC_CELL2PIXEL(8)+8, MGC_CELL2PIXEL(7)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[8].set_jump_check_interval(520);

    // top left1
    objs_.spectator()[9].spawn({MGC_CELL2PIXEL(6)+8, MGC_CELL2PIXEL(6)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type3);
    objs_.spectator()[9].set_jump_check_interval(290);
    objs_.spectator()[10].spawn({MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(6)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[10].set_jump_check_interval(320);
    objs_.spectator()[11].spawn({MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(6)}, true, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[11].set_jump_check_interval(450);

    // bottom right1
    objs_.spectator()[12].spawn({MGC_CELL2PIXEL(29), MGC_CELL2PIXEL(16)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[12].set_jump_check_interval(300);
    objs_.spectator()[13].spawn({MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(16)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type1);
    objs_.spectator()[13].set_jump_check_interval(410);
    objs_.spectator()[14].spawn({MGC_CELL2PIXEL(29)+8, MGC_CELL2PIXEL(16)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type3);
    objs_.spectator()[14].set_jump_check_interval(520);

    // bottom right1
    objs_.spectator()[15].spawn({MGC_CELL2PIXEL(31)+8, MGC_CELL2PIXEL(15)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type3);
    objs_.spectator()[15].set_jump_check_interval(290);
    objs_.spectator()[16].spawn({MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(15)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type1);
    objs_.spectator()[16].set_jump_check_interval(320);
    objs_.spectator()[17].spawn({MGC_CELL2PIXEL(32), MGC_CELL2PIXEL(15)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[17].set_jump_check_interval(350);

    // top right1
    objs_.spectator()[18].spawn({MGC_CELL2PIXEL(29), MGC_CELL2PIXEL(7)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[18].set_jump_check_interval(300);
    objs_.spectator()[19].spawn({MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(7)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[19].set_jump_check_interval(410);
    objs_.spectator()[20].spawn({MGC_CELL2PIXEL(29)+8, MGC_CELL2PIXEL(7)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type3);
    objs_.spectator()[20].set_jump_check_interval(520);

    // top right1
    objs_.spectator()[21].spawn({MGC_CELL2PIXEL(31)+8, MGC_CELL2PIXEL(6)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type1);
    objs_.spectator()[21].set_jump_check_interval(290);
    objs_.spectator()[22].spawn({MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(6)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[22].set_jump_check_interval(320);
    objs_.spectator()[23].spawn({MGC_CELL2PIXEL(32), MGC_CELL2PIXEL(6)}, false, enemy::SpectatorMode::Jump, enemy::SpectatorType::Type2);
    objs_.spectator()[23].set_jump_check_interval(450);

    set_event_state(EventState::Playing);
    unlock_control();
}

void Event_Stage3_6::spawn() {
    spawn({0, 0});
}

void Event_Stage3_6::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage3_6::update() {

    if ( event_state() != EventState::Playing ) return;

    switch (detail_) {
    case EventDetail::Idle:
        if ( player_.position().y <= MGC_CELL2PIXEL(17) ) {
            detail_ = EventDetail::Lookup;
            lock_control();
            player_.set_anim_mode(PlayerAnimMode::Manual);
            player_.set_anim_manually(PlayerAnimState::LookupRight, false);
            sw_.restart();
        }
        break;

    case EventDetail::Lookup:
        if ( sw_.elapsed_ms() >= 1987 ) {
            sw_.restart();
            sound_.play_sound_effect(MML_SE_8_OPEN);
            objs_.rocker().set_shadow_mode(false);
            objs_.spotlight1().turn_on();
            objs_.spotlight2().turn_on();

            detail_ = EventDetail::Appear;
            sw_.restart();
        }
        break;

    case EventDetail::Appear:
        
        if ( sw_.elapsed_ms() >= 1500 ) {
            sound_.play_background_music(MML_BGM_8_CHEER);
            detail_ = EventDetail::Cheer;
        }
        break;

    case EventDetail::Cheer:
        
        if ( sound_.has_background_music_finished() ) {
            sound_.play_background_music(MML_BGM_9_BOSS2);
            unlock_control();
            player_.set_anim_mode(PlayerAnimMode::Auto);
            detail_ = EventDetail::Fight;
        }
        break;

    case EventDetail::Fight:
        
        if ( n_popped_target_balloon_ < MAX_BALLOON_NUM_ ) {
            for ( size_t i = 0; i < objs_.target_balloons().size(); i++ ) {
                auto& balloon = objs_.target_balloons()[i];
                if ( balloon.state() == prop::TargetBalloon::State::Inactive ||
                     balloon.state() == prop::TargetBalloon::State::Despawned 
                ) {
                    if ( balloon.state() == prop::TargetBalloon::State::Despawned ) {
                        n_popped_target_balloon_++;
                        balloon.reset_despawned_state();
                    }
                    if ( n_created_balloon_ < MAX_BALLOON_NUM_ ) {
                        n_created_balloon_++;
                        switch (i) {
                        case 0:
                            balloon.spawn({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(7)});
                            balloon.set_goal({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(9)});
                            balloon.set_balloon_color(MGC_COLOR(1,0,0));//RED
                            balloon.set_gain({0.001, 0.002});
                            break;
                        case 1:
                            balloon.spawn({MGC_CELL2PIXEL(29)-8, MGC_CELL2PIXEL(7)});
                            balloon.set_goal({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(9)});
                            balloon.set_balloon_color(MGC_COLOR(0,1,0));//GREEN
                            balloon.set_gain({0.002, 0.001});
                            break;
                        case 2:
                            balloon.spawn({MGC_CELL2PIXEL(9)+8, MGC_CELL2PIXEL(0)});
                            balloon.set_goal({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(6)});
                            balloon.set_gain({0.0005, 0.0005});
                            balloon.set_balloon_color(MGC_COLOR(0,0,1));//BLUE
                            break;
                        case 3:
                            balloon.spawn({MGC_CELL2PIXEL(29), MGC_CELL2PIXEL(0)});
                            balloon.set_goal({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(6)});
                            balloon.set_gain({0.0003, 0.001});
                            balloon.set_balloon_color(MGC_COLOR(1,1,0));//YELLOW
                            break;
                        case 4:
                            balloon.spawn({MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(6)});
                            balloon.set_goal({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(7)});
                            balloon.set_gain({0.0005, 0.0001});
                            balloon.set_balloon_color(MGC_COLOR(1,0,1));//MAGENTA
                            break;
                        case 5:
                            balloon.spawn({MGC_CELL2PIXEL(32)-8, MGC_CELL2PIXEL(6)});
                            balloon.set_goal({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(7)});
                            balloon.set_gain({0.0003, 0.0005});
                            balloon.set_balloon_color(MGC_COLOR(0,1,1));//CYAN
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        } else {
            if (!finish_flag_) {
                finish_flag_ = true;
                sound_.finish_primary_loop(true);
            }

            if ( sound_.has_background_music_finished() ) {
                detail_ = EventDetail::Cheer2;
                sound_.play_background_music(MML_BGM_8_CHEER);
                sw_.restart();
                lock_control();
            }
        }

        switch ( objs_.rocker().rocker_state() ) {
        case enemy::RockerState::StepForward:
        case enemy::RockerState::StepBackward:
            //invite_skyfish({MGC_CELL2PIXEL(13), MGC_CELL2PIXEL(10)}, visit_[3], objs_.skyfish()[0]);
            //invite_skyfish({MGC_CELL2PIXEL(25), MGC_CELL2PIXEL(10)}, visit_[4], objs_.skyfish()[1]);
            //invite_balloon({MGC_CELL2PIXEL(12), MGC_CELL2PIXEL(15)}, visit_[5], objs_.balloon()[0]);
            //invite_balloon({MGC_CELL2PIXEL(24), MGC_CELL2PIXEL(14)}, visit_[6], objs_.balloon()[1]);
            /*@fall-through@*/

        case enemy::RockerState::Singing:
            invite_walker({MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(10)}, visit_[0], objs_.walker()[0]);
            invite_walker({MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(10)}, visit_[1], objs_.walker()[1]);
            invite_walker({MGC_CELL2PIXEL(21), MGC_CELL2PIXEL(10)}, visit_[2], objs_.walker()[2]);
            break;

        default:
            break;
        }
        break;

    case EventDetail::Cheer2:
        if ( sw_.elapsed_ms() >= 3000 ) {
            detail_ = EventDetail::Fadeout;
            screen_fader_.request_fade_out(ScreenFaderType::Fade);
        }
        break;

    case EventDetail::Fadeout:
        if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
            detail_ = EventDetail::Exit;
            objs_.all_enemy_despawn();
        }
        break;

    case EventDetail::Exit:
        if ( objs_.is_all_enemy_despawned() &&
             sound_.has_background_music_finished()
        ) {
            detail_ = EventDetail::Fadein;
            screen_fader_.request_fade_in(ScreenFaderType::Fade);
            objs_.spotlight1().turn_off();
            objs_.spotlight2().turn_off();
        }
        break;

    case EventDetail::Fadein:
        if ( screen_fader_.fade_state() == ScreenFaderState::FadeInComplete ) {
            detail_ = EventDetail::Victory;
            unlock_control();
            sound_.stop_background_music();
        }
        break;
    case EventDetail::Victory:
        cp_info_.advance_checkpoint(Checkpoint::WonTheStage3Boss);
        detail_ = EventDetail::Fadein;
        //objs_.stage().set_ladder_enabled(true);
        break;

    default:
        break;
    }
}

void Event_Stage3_6::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
    screen_fader_.update(fb);
}

} // namespace app

