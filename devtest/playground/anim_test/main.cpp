#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/anim_player.h"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"
#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"

namespace {

auto& gamepad = mgc::drivers::platform::input::default_gamepad();
mgc::drivers::platform::display::ST7789 st7789;
mgc::platform::timer::FrameTimer<mgc::drivers::platform::timer::FreeRunningTimerU32> frame_timer;

mgc_color_t buffer[240*240];
mgc::graphics::Framebuffer fb(buffer, 240, 240);

mgc::control::anim::AnimController<decltype(frame_timer)> anim(frame_timer);
mgc::parts::BasicSprite sprite;

}

int main() {

    st7789.init(50*1000*1000);
    gamepad.init();

    sprite.reset();
    sprite.set_position(mgc::math::Vec2i{0, 0});

    anim.reset();
    anim.set_anim_frames(anim_player_walk_left);
    anim.set_loop(true);
    anim.set_current_frame(sprite);

    frame_timer.reset();

    anim.start_animation();

    while (1) {

        frame_timer.tick();

        gamepad.proc();

        if ( gamepad.just_pressed(mgc::platform::input::Key::Right) ) {
            anim.set_anim_frames(anim_player_walk_right);
            anim.start_animation();
        } else if ( gamepad.just_pressed(mgc::platform::input::Key::Left) ) {
            anim.set_anim_frames(anim_player_walk_left);
            anim.start_animation();
        } else { }


        if ( !gamepad.is_pressed(mgc::platform::input::Key::Right) &&
             !gamepad.is_pressed(mgc::platform::input::Key::Left)
        ) {
            anim.pause_animation();
        }


        if ( anim.proc() ) {
            anim.set_current_frame(sprite);
        }

        fb.clear();

        sprite.draw(fb);

        st7789.transfer_full_region_blocking((uint8_t*)buffer, sizeof(buffer));
    }

    return 0;
}
