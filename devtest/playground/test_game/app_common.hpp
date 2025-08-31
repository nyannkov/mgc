#ifndef MGC_APP_COMMON_HPP
#define MGC_APP_COMMON_HPP

#include <type_traits>
#include "mgc_cpp/mgc.hpp"
#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"
#include "mgc_drivers/platform/sound/mml_psg/cpp/sound_controller_mml_psg.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"

namespace app {

// Alias
using GamepadT = std::remove_reference_t<decltype(mgc::drivers::platform::input::default_gamepad())>;
using DisplayDriverT = mgc::drivers::platform::display::ST7789;
using SoundControllerT = mgc::drivers::platform::sound::MmlPsgSoundController;
using HwTimerT = mgc::drivers::platform::timer::FreeRunningTimerU32;
using FrameTimerT = mgc::platform::timer::FrameTimer<HwTimerT>;

using BTreeControllerT = mgc::control::btree::BTreeController<FrameTimerT>;
using IBTListenerT =  mgc::control::btree::IBTreeListener<BTreeControllerT>;
using AnimControllerT = mgc::control::anim::AnimController<FrameTimerT>;

// Utility
struct StopWatch {
    explicit StopWatch(const FrameTimerT& frame_timer)
        : frame_timer_(frame_timer) { reset(); }
    ~StopWatch() = default;

    void reset() {
        base_ms_ = 0;
        elapsed_ms_ = 0;
        running_ = false;
    }

    void start() {
        base_ms_ = frame_timer_.now_ms();
        running_ = true;
    }

    void stop() {
        running_ = false;
    }

    FrameTimerT::timestamp_t elapsed_ms() {
        if ( running_ ) {
            elapsed_ms_ = frame_timer_.now_ms() - base_ms_;
        }
        return elapsed_ms_;
    }

    void restart() {
        base_ms_ = frame_timer_.now_ms();
        elapsed_ms_ = 0;
        running_ = true;
    }
    
private:
    const FrameTimerT& frame_timer_;
    FrameTimerT::timestamp_t base_ms_;
    FrameTimerT::timestamp_t elapsed_ms_;
    bool running_;
};


template <typename T, typename IndexT, size_t N>
constexpr T& at(std::array<T, N>& arr, IndexT index) noexcept {
    return arr[static_cast<size_t>(index)];
}

template <typename T, typename IndexT, size_t N>
constexpr const T& at(const std::array<T, N>& arr, IndexT index) noexcept {
    return arr[static_cast<size_t>(index)];
}

} // namespace app

#endif/*MGC_APP_COMMON_HPP*/

