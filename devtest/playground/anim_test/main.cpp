#include "mgc/mgc.h"
#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/anim_player.h"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"
#include "mgc_drivers/platform/display/st7789/st7789.h"
#include "mgc_drivers/platform/input/digital_gamepad/digital_gamepad.h"

namespace {
mgc_sprite_t sprite;
mgc_animctrl_t animctrl;
mgc_framebuffer_t fb;
mgc_color_t buffer[240*240];
mgc_digital_gamepad_t gamepad;

mgc::platform::timer::FrameTimer<mgc::drivers::platform::timer::FreeRunningTimerU32> frame_timer;

}

int main() {

    st7789_init(50*1000*1000);

    digital_gamepad_init_default_handler();
    digital_gamepad_copy_default_handler(&gamepad);

    framebuffer_init(&fb, buffer, 240, 240);

    animctrl_init(&animctrl);
    animctrl_set_anim_frames(&animctrl, &anim_player_walk_left);
    animctrl_set_loop(&animctrl, true);

    sprite_init(&sprite, 0);
    sprite_set_position(&sprite, 0, 0);
    sprite_set_tileset(&sprite, animctrl_get_tileset(&animctrl));
    sprite_set_tile_index(&sprite, animctrl_get_current_tile_index(&animctrl));

    frame_timer.reset();

    uint32_t timer_now = frame_timer.now_ms32();

    animctrl_start(&animctrl, timer_now);

    while (1) {

        frame_timer.tick();

        timer_now = frame_timer.now_ms32();

        digital_gamepad_proc(&gamepad);

        if ( digital_gamepad_is_pressed(&gamepad, MGC_DIGITAL_GAMEPAD_KEY_RIGHT) ) {
            if ( !digital_gamepad_was_pressed(&gamepad, MGC_DIGITAL_GAMEPAD_KEY_RIGHT) ) {
                animctrl_set_anim_frames(&animctrl, &anim_player_walk_right);
                animctrl_start(&animctrl, timer_now);
            }
        } else if ( digital_gamepad_is_pressed(&gamepad, MGC_DIGITAL_GAMEPAD_KEY_LEFT) ) {
            if ( !digital_gamepad_was_pressed(&gamepad, MGC_DIGITAL_GAMEPAD_KEY_LEFT) ) {
                animctrl_set_anim_frames(&animctrl, &anim_player_walk_left);
                animctrl_start(&animctrl, timer_now);
            }
        } else {
            animctrl_pause(&animctrl);
        }

        if ( animctrl_proc(&animctrl, timer_now) ) {
            sprite_set_tile_index(&sprite, animctrl_get_current_tile_index(&animctrl));
        }

        framebuffer_clear(&fb, MGC_COLOR_BLACK);

        sprite_draw(&sprite, &fb, NULL, NULL);

        st7789_transfer_full_region_blocking_rgb565((uint8_t*)buffer, sizeof(buffer));
    }

    return 0;
}
