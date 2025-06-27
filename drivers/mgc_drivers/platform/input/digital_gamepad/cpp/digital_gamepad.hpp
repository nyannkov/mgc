/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_DIGITAL_GAMEPAD_HPP
#define MGC_DRIVERS_DIGITAL_GAMEPAD_HPP

#include "mgc_cpp/platform/input/ibutton.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/digital_gamepad.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace input {

struct DigitalGamepadKeyMapper {
    static enum mgc_digital_gamepad_key to_digital_gamepad_key(mgc::platform::input::Key key) {
        switch (key) {
        case mgc::platform::input::Key::Up:         return MGC_DIGITAL_GAMEPAD_KEY_UP;
        case mgc::platform::input::Key::Down:       return MGC_DIGITAL_GAMEPAD_KEY_DOWN;
        case mgc::platform::input::Key::Left:       return MGC_DIGITAL_GAMEPAD_KEY_LEFT;
        case mgc::platform::input::Key::Right:      return MGC_DIGITAL_GAMEPAD_KEY_RIGHT;
        case mgc::platform::input::Key::Enter:      return MGC_DIGITAL_GAMEPAD_KEY_ENTER;
        case mgc::platform::input::Key::Cancel:     return MGC_DIGITAL_GAMEPAD_KEY_CANCEL;
        case mgc::platform::input::Key::Menu:       return MGC_DIGITAL_GAMEPAD_KEY_MENU;
        case mgc::platform::input::Key::Home:       return MGC_DIGITAL_GAMEPAD_KEY_HOME;
        case mgc::platform::input::Key::Control:    return MGC_DIGITAL_GAMEPAD_KEY_CONTROL;
        default:
            return MGC_DIGITAL_GAMEPAD_KEY_COUNT;
        }
    }
};

using DigitalGamepadPinConfig = mgc_digital_gamepad_config_t;

template <size_t NumKeys>
struct DigitalGamepad : mgc::platform::input::IButton {
    static_assert(NumKeys > 0, "NumKeys must be greater than 0");
    static_assert(NumKeys <= MGC_DIGITAL_GAMEPAD_KEY_COUNT, "Too many keys");

    explicit DigitalGamepad(const DigitalGamepadPinConfig (&config_table)[NumKeys])
        : config_table_(config_table) {
        for (size_t i = 0; i < NumKeys; ++i) {
            state_table_[i] = MGC_DIGITAL_GAMEPAD_INIT_STATE(&config_table_[i]);
        }
    }
    DigitalGamepad(const DigitalGamepad&) = delete;
    DigitalGamepad& operator=(const DigitalGamepad&) = delete;
    DigitalGamepad(DigitalGamepad&&) = default;
    DigitalGamepad& operator=(DigitalGamepad&&) = default;

    void init() {
        digital_gamepad_init(&gamepad_, state_table_, NumKeys);
    }

    void proc() {
        digital_gamepad_proc(&gamepad_);
    }

    uint16_t hold_counter(mgc::platform::input::Key key) const override {
        return digital_gamepad_get_hold_counter(&gamepad_, DigitalGamepadKeyMapper::to_digital_gamepad_key(key));
    }

    bool is_pressed(mgc::platform::input::Key key) const override {
        return digital_gamepad_is_pressed(&gamepad_, DigitalGamepadKeyMapper::to_digital_gamepad_key(key));
    }

    bool was_pressed(mgc::platform::input::Key key) const override {
        return digital_gamepad_was_pressed(&gamepad_, DigitalGamepadKeyMapper::to_digital_gamepad_key(key));
    }

    using mgc::platform::input::IButton::just_pressed;
    using mgc::platform::input::IButton::just_released;

private:
    const DigitalGamepadPinConfig (&config_table_)[NumKeys];
    mgc_digital_gamepad_state_t state_table_[NumKeys]{};
    mgc_digital_gamepad_t gamepad_{};
};


inline auto create_default_gamepad() {
    static_assert(MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT > 0, "Default key count must be > 0");
    static_assert(MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT <= MGC_DIGITAL_GAMEPAD_KEY_COUNT, "Too many default keys");

    return DigitalGamepad<MGC_DIGITAL_GAMEPAD_DEFAULT_KEY_COUNT>(
        mgc_digital_gamepad_default_config_table
    );
}

inline auto& default_gamepad() {
    static auto instance = create_default_gamepad();
    return instance;
}


}// namespace input
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_DRIVERS_DIGITAL_GAMEPAD_HPP*/
