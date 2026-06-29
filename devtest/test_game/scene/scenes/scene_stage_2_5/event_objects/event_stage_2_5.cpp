#include <cstdio>
#include "resources/mml/mml.h"
#include "event_stage_2_5.hpp"

namespace app {

using app::event::EventState;

Event_Stage2_5::Event_Stage2_5(
    SceneContext& scx,
    SceneObjects_Stage2_5& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    label_counter_(objs.label_counter()),
    buttons_(objs.buttons()) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);

    mgc::math::Vec2i pos = {MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(14)};
    for ( size_t i = 0; i < buttons_.size(); ++i ) {
        buttons_[i].spawn(pos, false);
        pos.x += 32;
    }
}

void Event_Stage2_5::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    unlock_control();

    counter_ = 7;
    snprintf(counter_buffer_, sizeof(counter_buffer_), "%d", counter_);
    label_counter_.set_text(counter_buffer_);

    wait_flag_ = false;
    for ( size_t i = 0; i < buttons_.size(); ++i ) {
        buttons_[i].set_pushed_state(false);
    }
}

void Event_Stage2_5::spawn() {
    spawn({0, 0});
}

void Event_Stage2_5::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Stage2_5::update() {

    size_t pushed_count = 0;
    
    if ( event_state() != EventState::Playing ) {
        return;
    }

    if ( wait_flag_ ) {
        if ( sound_.have_all_sound_effects_finished() ) {
            wait_flag_ = false;
            cp_info_.advance_checkpoint(Checkpoint::Puzzle1_Solved);
            unlock_control();
            set_event_state(EventState::Finished);
        }
        return;
    }

    for ( size_t i = 0; i < buttons_.size(); ++i ) {
        if ( buttons_[i].just_pushed() ) {
            size_t left = (buttons_.size() + i - 1) % buttons_.size();
            size_t right = (buttons_.size() + i + 1) % buttons_.size();
            buttons_[left].toggle_state();
            buttons_[right].toggle_state();
            if ( -999 < counter_ ) {
                counter_--;
            }
            if ( 3 <= counter_  ) {
                label_counter_.set_font_fore_color(MGC_COLOR(0,1,0));
            } else if ( 0 < counter_ && counter_ < 3 ) {
                label_counter_.set_font_fore_color(MGC_COLOR(1,1,0));
            } else if ( 0 == counter_ ) {
                label_counter_.set_font_fore_color(MGC_COLOR(1,1,1));
            } else { 
                label_counter_.set_font_fore_color(MGC_COLOR(1,0,0));
            }
            snprintf(counter_buffer_, sizeof(counter_buffer_), "%d", counter_);
            break;
        }
    }
    for ( size_t i = 0; i < buttons_.size(); ++i ) {
        if ( buttons_[i].is_pushed() ) {
            pushed_count++;
        }
    }

    if ( counter_ == 0 ) {
        if ( pushed_count == buttons_.size() ) {
            // Success
            sound_.play_sound_effect(MML_SE_13_DISCHARGE);
            lock_control();
            wait_flag_ = true;
        } 
    }
}

void Event_Stage2_5::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
}

} // namespace app

