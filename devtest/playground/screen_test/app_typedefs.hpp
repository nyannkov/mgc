#ifndef MGC_APP_TYPEDEFS_HPP
#define MGC_APP_TYPEDEFS_HPP

#include "mgc_cpp/mgc.hpp"
#include "platform.hpp"

#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"
#include "mgc_drivers/platform/sound/mml_psg/cpp/sound_controller_mml_psg.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"

namespace app {

using InputT = decltype(mgc::drivers::platform::input::default_gamepad());
using DisplayT = mgc::drivers::platform::display::ST7789;
using SoundT = mgc::drivers::platform::sound::MmlPsgSoundController;
using TimerT = mgc::drivers::platform::timer::FreeRunningTimerU32;

using PlatformT = Platform<InputT, SoundT, DisplayT, TimerT>;

using BTreeControllerT = mgc::control::btree::BTreeController<PlatformT::TimerT>;
using IBTreeListenerT = mgc::control::btree::IBTreeListener<BTreeControllerT>;


}// namespace app

#endif/*MGC_APP_TYPEDEFS_HPP*/

