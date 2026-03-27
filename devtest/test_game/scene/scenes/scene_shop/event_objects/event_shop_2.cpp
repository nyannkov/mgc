#include "event_shop_2.hpp"

namespace app {

using app::event::EventState;

Event_Shop2::Event_Shop2(
    SceneContext& scx,
    SceneObjects_Shop& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    player_(scx.player),
    florist_(objs.florist()),
    chair_left_(objs.chair_left()),
    objs_(objs) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
    unlock_control();
}

void Event_Shop2::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    unlock_control();
    screen_fader_.clear();
    screen_fader_.set_fade_speed(5);
    screen_fader_.set_fill_color(MGC_COLOR_BLACK);
    detail_ = EventDetail::Stop;
}

void Event_Shop2::spawn() {
    spawn({0, 0});
}

void Event_Shop2::despawn() {
    set_event_state(EventState::NotStarted);
}

void Event_Shop2::update() {
    if ( event_state() == EventState::Playing ) {
        switch (detail_) {
        case EventDetail::Stop:
            if ( florist_.coffee_break_flag() ) {
                florist_.clear_coffee_break_flag();
                lock_control();
                detail_ = EventDetail::FadeOut;
                screen_fader_.request_fade_out(ScreenFaderType::Fade);
            }
            break;

        case EventDetail::FadeOut:
            if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
                detail_ = EventDetail::FadeIn;
                chair_left_.set_position({MGC_CELL2PIXEL(8)-8, MGC_CELL2PIXEL(6)});
                player_.set_position({MGC_CELL2PIXEL(8)-8, MGC_CELL2PIXEL(6)});
                player_.set_anim_mode(PlayerAnimMode::Manual);
                player_.set_anim_manually(PlayerAnimState::CoffeeBreak, false);
                screen_fader_.request_fade_in(ScreenFaderType::Fade);
                objs_.set_table_layer(true);
            }
            break;

        case EventDetail::FadeIn:
            if ( screen_fader_.fade_state() == ScreenFaderState::FadeInComplete ) {
                detail_ = EventDetail::DrinkCoffee;
            }
            break;

        case EventDetail::DrinkCoffee:
            if ( player_.is_animation_finished() ) {
                unlock_control();//TODO
                set_and_trigger_talkflow_request({
                    &talkscript_1,
                    MGC_TALKSCRIPT_1_SHOP_COFFEE_BREAK_2,
                    &talkflow_listener_,
                    TalkflowEffectType::Mute
                });
                detail_ = EventDetail::WaitFadeOut2;
            }
            break;
        case EventDetail::WaitFadeOut2:
            if ( talkflow_listener_.is_finished() ) {
                talkflow_listener_.clear();
                lock_control();//TODO
                detail_ = EventDetail::FadeOut2;
                screen_fader_.request_fade_out(ScreenFaderType::Fade);
                player_.receive_heal(player_.full_hp());
            }
            break;
        case EventDetail::FadeOut2:
            if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
                player_.set_anim_manually(PlayerAnimState::StandRight);
                objs_.set_table_layer(false);
                player_.set_position({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(6)});
                detail_ = EventDetail::FadeIn2;
                screen_fader_.request_fade_in(ScreenFaderType::Fade);
            }
            break;
        case EventDetail::FadeIn2:
            if ( screen_fader_.fade_state() == ScreenFaderState::FadeInComplete ) {
                player_.set_anim_mode(PlayerAnimMode::Auto);
                detail_ = EventDetail::Stop;
                unlock_control();//TODO
            }
            break;
        }
    }
}

void Event_Shop2::draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
    screen_fader_.update(fb);
}

} // namespace app

