#ifndef MGC_PLATFORM_CONF_HPP
#define MGC_PLATFORM_CONF_HPP

#include "mgc_cpp/mgc.hpp"

#if defined(MGC_SIMULATION)
#include "display_sdl2.hpp"
#include "gamepad_sdl2.hpp"
#else
#include "st7789.hpp"
#include "gamepad_picolcd.hpp"
#endif

#include "sound_wrap.hpp"
#include "free_running_timer_u32.hpp"

namespace app {

#if defined(MGC_SIMULATION)
using DisplayDriverT = mgc::drivers::platform::display::DisplaySDL2;
using GamepadT = mgc::drivers::platform::input::GamepadSDL2;
#else
using DisplayDriverT = mgc::drivers::platform::display::ST7789;
using GamepadT = mgc::drivers::platform::input::GamepadPicoLcd;
#endif

using SoundControllerT = app::SoundControllerWrapper;
using HwTimerT = mgc::drivers::platform::timer::FreeRunningTimerU32;
using FrameTimerT = mgc::platform::timer::FrameTimer<HwTimerT>;

}// namespace app

#endif/*MGC_PLATFORM_CONF_HPP*/
