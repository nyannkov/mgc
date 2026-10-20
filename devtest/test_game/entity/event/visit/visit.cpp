#include "visit.hpp"
#include "entity/enemy/enemy.hpp"
#include "resources/generated/anim/effect/vortex/anim_effect_vortex.h"

namespace app {

using app::event::EventState;

Visit::Visit(const FrameTimerT& timer) 
    : anim_vortex_(timer),
      sw_(timer) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
}

void Visit::spawn(const mgc::math::Vec2i& pos) {
    unlock_control();

    this->mut_sprite().set_visible(true);
    this->set_position(pos);

    anim_vortex_.set_anim_frames(anim_effect_vortex_small_vortex);
    anim_vortex_.start_animation();
    anim_vortex_.set_loop(true);
    anim_vortex_.set_current_frame(this->mut_sprite());

    visit_state_ = VisitState::Stopped;

    set_event_state(EventState::Playing);
}

void Visit::despawn() {
    set_event_state(EventState::NotStarted);
}

void Visit::update() {
    if ( this->event_state() == EventState::Playing ) {
        VisitState next = visit_state_;

        switch ( visit_state_ ) {
        case VisitState::Stopped:
            this->mut_sprite().set_visible(true);
            next = VisitState::FadeIn;
            anim_vortex_.start_animation();
            sw_.restart();
            break;
        case VisitState::FadeIn:
            if (sw_.elapsed_ms() >= 1000 ) {
                sw_.restart();
                next = VisitState::Spawning;
                anim_vortex_.set_anim_frames(anim_effect_vortex_large_vortex);
                anim_vortex_.set_loop(true);
                anim_vortex_.start_animation();
                if ( visitor_ ) {
                    visitor_->spawn(this->position(), false);//TODO
                }
            }
            break;
        case VisitState::Spawning:
            if (sw_.elapsed_ms() >= 2000 ) {
                sw_.restart();
                next = VisitState::FadeOut;
                anim_vortex_.set_anim_frames(anim_effect_vortex_small_vortex);
                anim_vortex_.set_loop(true);
                anim_vortex_.start_animation();
            }
            break;
        case VisitState::FadeOut:
            if (sw_.elapsed_ms() >= 1000 ) {
                sw_.restart();
                next = VisitState::End;
            }
            break;
        case VisitState::End:
            this->mut_sprite().set_visible(false);
            set_event_state(EventState::Finished);
            break;
        }

        if ( visit_state_ != next ) {
            visit_state_ = next;
        }
        
        anim_vortex_.proc();
        anim_vortex_.set_current_frame(this->mut_sprite());
    }
}

void Visit::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
    this->sprite().draw(fb, cam_pos);
}

} // namespace app


