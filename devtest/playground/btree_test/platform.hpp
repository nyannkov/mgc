#ifndef MGC_PLATFORM_HPP
#define MGC_PLATFORM_HPP

#include "mgc_cpp/platform/input/ibutton.hpp"
#include "mgc_cpp/platform/sound/isound_controller.hpp"
#include "mgc_cpp/platform/display/display_driver.hpp"
#include "mgc_cpp/platform/timer/frame_timer.hpp"

namespace devtest {

template <
    typename InputImplT,
    typename SoundImplT,
    typename DisplayImplT,
    typename FrameTimerImplT
>
struct Platform {

    Platform(InputImplT& input, SoundImplT& sound, DisplayImplT& display, FrameTimerImplT& timer) :
        input_(input),
        sound_(sound),
        display_(display),
        timer_(timer)
    {}

    using timestamp_t = typename FrameTimerImplT::timestamp_t;

    using InputT = InputImplT;
    using FrameTimerT = FrameTimerImplT;
    using SoundT = SoundImplT;
    using DisplayT =DisplayImplT;

    ~Platform() = default;
    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;
    Platform(Platform&&) = default;
    Platform& operator=(Platform&&) = default;


    mgc::platform::input::IButton& input() {
        return input_;
    }

    const mgc::platform::input::IButton& input() const {
        return input_;
    }

    mgc::platform::sound::ISoundController& sound() {
        return sound_;
    }

    const mgc::platform::sound::ISoundController& sound() const {
        return sound_;
    }

    mgc::platform::display::DisplayDriver<DisplayImplT>& display() {
        return display_;
    }

    const mgc::platform::display::DisplayDriver<DisplayImplT>& display() const {
        return display_;
    }

    mgc::platform::timer::FrameTimer<FrameTimerImplT>& timer() {
        return timer_;
    }

    const mgc::platform::timer::FrameTimer<FrameTimerImplT>& timer() const {
        return timer_;
    }

    InputImplT& input_impl() {
        return input_;
    }

    const InputImplT& input_impl() const {
        return input_;
    }

    SoundImplT& sound_impl() {
        return sound_;
    }

    const SoundImplT& sound_impl() const {
        return sound_;
    }

    DisplayImplT& display_impl() {
        return display_;
    }

    const DisplayImplT& display_impl() const {
        return display_;
    }

    FrameTimerImplT& timer_impl() {
        return timer_;
    }

    const FrameTimerImplT& timer_impl() const {
        return timer_;
    }

private:
    InputImplT& input_;
    SoundImplT& sound_;
    DisplayImplT& display_;
    FrameTimerImplT& timer_;
};

} // namepsace devtest

#endif/*MGC_PLATFORM_HPP*/

