#ifndef MGC_PLATFORM_HPP
#define MGC_PLATFORM_HPP

#include "platform_conf.hpp"

namespace app {

inline DisplayDriverT display_driver;
inline SoundControllerT sound_controller;
inline GamepadT gamepad;
inline FrameTimerT frame_timer;

extern void platform_init();
extern void platform_sleep(unsigned int ms);
extern void platform_gamepad_proc();
extern void platform_sound_proc();
extern void platform_tick();
extern bool platform_is_continue();
extern void platform_deinit();

}// namespace app

#endif/*MGC_PLATFORM_HPP*/
