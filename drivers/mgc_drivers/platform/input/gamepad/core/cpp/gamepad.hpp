/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_GAMEPAD_HPP
#define MGC_DRIVERS_GAMEPAD_HPP

#include "mgc_cpp/platform/input/ibutton.hpp"
#include "gamepad.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace input {

struct GamepadKeyMapper {
    static enum mgc_gamepad_key to_gamepad_key(mgc::platform::input::Key key) {
        switch (key) {
        case mgc::platform::input::Key::Up:         return MGC_GAMEPAD_KEY_UP;
        case mgc::platform::input::Key::Down:       return MGC_GAMEPAD_KEY_DOWN;
        case mgc::platform::input::Key::Left:       return MGC_GAMEPAD_KEY_LEFT;
        case mgc::platform::input::Key::Right:      return MGC_GAMEPAD_KEY_RIGHT;
        case mgc::platform::input::Key::Enter:      return MGC_GAMEPAD_KEY_ENTER;
        case mgc::platform::input::Key::Cancel:     return MGC_GAMEPAD_KEY_CANCEL;
        case mgc::platform::input::Key::Menu:       return MGC_GAMEPAD_KEY_MENU;
        case mgc::platform::input::Key::Home:       return MGC_GAMEPAD_KEY_HOME;
        case mgc::platform::input::Key::Control:    return MGC_GAMEPAD_KEY_CONTROL;
        default:
            return MGC_GAMEPAD_KEY_UNKNOWN;
        }
    }
};

struct Gamepad : mgc::platform::input::IButton {
    Gamepad() {}
    ~Gamepad() = default;

    void bind(mgc_gamepad_t& gamepad) {
        gamepad_ = &gamepad;
    }

    void unbind() {
        gamepad_ = nullptr;
    }

    void poll() {
        gamepad_poll(gamepad_);
    }

    uint16_t hold_counter(mgc::platform::input::Key key) const override {
        return gamepad_get_hold_counter(gamepad_, GamepadKeyMapper::to_gamepad_key(key));
    }

    bool is_pressed(mgc::platform::input::Key key) const override {
        return gamepad_is_pressed(gamepad_, GamepadKeyMapper::to_gamepad_key(key));
    }

    bool was_pressed(mgc::platform::input::Key key) const override {
        return gamepad_was_pressed(gamepad_, GamepadKeyMapper::to_gamepad_key(key));
    }

    using mgc::platform::input::IButton::just_pressed;
    using mgc::platform::input::IButton::just_released;

private:
    mgc_gamepad_t* gamepad_;
};

}// namespace input
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_DRIVERS_GAMEPAD_HPP*/
