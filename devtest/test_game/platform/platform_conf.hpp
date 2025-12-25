#ifndef MGC_PLATFORM_CONF_HPP
#define MGC_PLATFORM_CONF_HPP

#include "mgc_cpp/mgc.hpp"

#if defined(MGC_SIMULATION)
#include "simulation/drivers/display_sim.hpp"
#else
#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#endif

#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"
#include "mgc_drivers/platform/sound/mml_psg/cpp/sound_controller_mml_psg.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"

namespace app {

using GamepadT = std::remove_reference_t<decltype(mgc::drivers::platform::input::default_gamepad())>;

#if defined(MGC_SIMULATION)
using DisplayDriverT = mgc::drivers::platform::display::DisplaySim;
#else
using DisplayDriverT = mgc::drivers::platform::display::ST7789;
#endif

using SoundControllerT = mgc::drivers::platform::sound::MmlPsgSoundController;

using HwTimerT = mgc::drivers::platform::timer::FreeRunningTimerU32;

using FrameTimerT = mgc::platform::timer::FrameTimer<HwTimerT>;

}// namespace app

#endif/*MGC_PLATFORM_CONF_HPP*/
