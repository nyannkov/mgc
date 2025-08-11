#ifndef MGC_PLATFORM_HPP
#define MGC_PLATFORM_HPP

#include "mgc_cpp/platform/input/ibutton.hpp"
#include "mgc_cpp/platform/sound/isound_controller.hpp"
#include "mgc_cpp/platform/display/display_driver.hpp"

namespace app {

template <typename InputImplT, typename SoundImplT, typename DisplayImplT>
struct Platform {

    Platform(InputImplT& input, SoundImplT& sound, DisplayImplT& display) :
        input_(input),
        sound_(sound),
        display_(display) {}

    using InputT = InputImplT;
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

private:
    InputImplT& input_;
    SoundImplT& sound_;
    DisplayImplT& display_;
};

} // namepsace app

#endif/*MGC_PLATFORM_HPP*/

