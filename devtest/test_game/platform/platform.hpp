#ifndef MGC_PLATFORM_HPP
#define MGC_PLATFORM_HPP

#include "platform_conf.hpp"

namespace app {

struct Platform {
    DisplayDriverT display_driver;
    SoundControllerT sound_controller;
    GamepadT gamepad;
    FrameTimerT frame_timer;
};

extern void platform_init();
extern void platform_sleep(unsigned int ms);
extern void platform_gamepad_proc();
extern void platform_sound_proc();
extern void platform_tick();
extern bool platform_is_continue();
extern void platform_deinit();

inline Platform platform;

}// namespace app

#endif/*MGC_PLATFORM_HPP*/
