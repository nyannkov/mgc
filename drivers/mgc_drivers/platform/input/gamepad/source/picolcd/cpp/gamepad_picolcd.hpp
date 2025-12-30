/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_GAMEPAD_PICOLCD_HPP
#define MGC_DRIVERS_GAMEPAD_PICOLCD_HPP

#include "gamepad.hpp"
#include "gamepad_picolcd.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace input {

struct GamepadPicoLcd : Gamepad {
    GamepadPicoLcd() {};
    ~GamepadPicoLcd() = default;
    GamepadPicoLcd(const GamepadPicoLcd&) = delete;
    GamepadPicoLcd& operator=(const GamepadPicoLcd&) = delete;
    GamepadPicoLcd(GamepadPicoLcd&&) = default;
    GamepadPicoLcd& operator=(GamepadPicoLcd&&) = default;

    void init() {
        gamepad_picolcd_init(&gamepad_);
        bind(gamepad_);
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

#endif/*MGC_DRIVERS_GAMEPAD_PICOLCD_HPP*/
