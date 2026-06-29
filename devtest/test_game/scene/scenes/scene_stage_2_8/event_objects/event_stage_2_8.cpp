#include "resources/mml/mml.h"
#include "event_stage_2_8.hpp"
#include "event_stage_2_8_common.hpp"

namespace app {

using app::event::EventState;

Event_Stage2_8::Event_Stage2_8(
    SceneContext& scx,
    SceneObjects_Stage2_8& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    objs_(objs),
    sw_(scx.timer) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_8::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);

    objs_.stage().set_water_enabled(true);
    auto water_pos = objs_.stage().water_position();
    water_pos.y = MAX_WATER_LEVEL;
    objs_.stage().set_water_position(water_pos);


    detail_ = EventDetail::NotStarted;
    counter_ = 0;
    button_pos_case_ = 0;

    objs_.button().spawn({MGC_CELL2PIXEL(66), MGC_CELL2PIXEL(1)}, true);
    objs_.button().set_pushed_state(false);
}

void Event_Stage2_8::spawn() {
    spawn({0, 0});
}

void Event_Stage2_8::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_8::update() {
    
    {
        auto button_pos = objs_.button().position();
        if ( objs_.button().is_pushed() ) {
            if ( button_pos.y > 0 ) {
                button_pos.y--;
            }
        } else {
            if ( button_pos.y < MGC_CELL2PIXEL(1) ) {
                button_pos.y++;
            }
        }
        objs_.button().set_position(button_pos);
    }

    switch (event_state()) {
    case EventState::Playing:
        if ( detail_ == EventDetail::NotStarted ) {
            if ( objs_.button().is_pushed() ) {
                sound_.play_sound_effect(MML_SE_15_DISCHARGE_3);
                detail_ = EventDetail::StartToDischarge;
                counter_ = 0;
                sw_.restart();
            }
        } else {
            
            if ( detail_ == EventDetail::StartToDischarge ) {
                if ( sw_.elapsed_ms() >= 1000 ) {
                    detail_ = EventDetail::Discharging;
                }
            } else if ( detail_ == EventDetail::Discharging ) {
                counter_++;
                if ( counter_ & 0x1 ) {
                    auto pos = objs_.stage().water_position();
                    pos.y += 1;
                    objs_.stage().set_water_position(pos);
                    if ( sound_.have_all_sound_effects_finished() && 
                         pos.y >= MIN_WATER_LEVEL
                    ) {
                        detail_ = EventDetail::Discharged;
                        //set_event_state(EventState::Finished);
                        sw_.restart();
                    }
                }
            } else if ( detail_ == EventDetail::Discharged ) {
                if ( sw_.elapsed_ms() >= 10000 ) {
                    detail_ = EventDetail::Charging;
                    counter_ = 0;
                    auto button_pos = objs_.button().position();
                    switch ( button_pos_case_ ) {
                    case 0:
                        button_pos_case_ = 1;
                        button_pos.x = MGC_CELL2PIXEL(34);
                        break;
                    case 1:
                        button_pos_case_ = 2;
                        button_pos.x = MGC_CELL2PIXEL(9);
                        break;
                    case 2:
                    default:
                        button_pos_case_ = 0;
                        button_pos.x = MGC_CELL2PIXEL(66);
                        break;
                    }
                    objs_.button().set_position(button_pos);
                }
            } else if ( detail_ == EventDetail::Charging ) {
                counter_++;
                if ( counter_ & 0x1 ) {
                    auto pos = objs_.stage().water_position();
                    pos.y -= 1;
                    objs_.stage().set_water_position(pos);
                    if ( sound_.have_all_sound_effects_finished() && 
                         pos.y <= MAX_WATER_LEVEL 
                    ) {
                        objs_.button().set_pushed_state(false);
                        set_event_state(EventState::Finished);
                    }
                }
            } else { }
        } 
        break;

    case EventState::Finished:
        set_event_state(EventState::Playing);
        detail_ = EventDetail::NotStarted;
        break;

    default:
        break;
    }
}

} // namespace app

