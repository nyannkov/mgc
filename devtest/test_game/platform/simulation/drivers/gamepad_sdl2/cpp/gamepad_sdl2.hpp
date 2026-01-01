#ifndef MGC_DRIVERS_GAMEPAD_SDL2_HPP
#define MGC_DRIVERS_GAMEPAD_SDL2_HPP

#include "gamepad.hpp"
#include "gamepad_sdl2.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace input {

struct GamepadSDL2 : Gamepad {
    GamepadSDL2() : Gamepad(gamepad_) {};
    ~GamepadSDL2() = default;
    GamepadSDL2(const GamepadSDL2&) = delete;
    GamepadSDL2& operator=(const GamepadSDL2&) = delete;
    GamepadSDL2(GamepadSDL2&&) = default;
    GamepadSDL2& operator=(GamepadSDL2&&) = default;

    void init() {
        gamepad_sdl2_init(&gamepad_);
    }

    using mgc::platform::input::IButton::just_pressed;
    using mgc::platform::input::IButton::just_released;

private:
    mgc_gamepad_t gamepad_;
};

}// namespace input
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_DRIVERS_GAMEPAD_SDL2_HPP*/
