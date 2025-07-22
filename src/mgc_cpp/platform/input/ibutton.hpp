/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PLATFORM_INPUT_IBUTTON_HPP
#define MGC_PLATFORM_INPUT_IBUTTON_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/platform/input/key.hpp"

namespace mgc {
namespace platform {
namespace input {

struct IButton {

    virtual ~IButton() = default;

    virtual bool is_pressed(mgc::platform::input::Key key) const = 0;
    virtual bool was_pressed(mgc::platform::input::Key key) const = 0;
    virtual uint16_t hold_counter(mgc::platform::input::Key key) const = 0;

    bool just_pressed(mgc::platform::input::Key key) const {
        return ( is_pressed(key) && !was_pressed(key) );
    }
    bool just_released(mgc::platform::input::Key key) const {
        return ( !is_pressed(key) && was_pressed(key) );
    }
};

inline bool is_any_button_pressed(const IButton& button) {
    constexpr mgc::platform::input::Key all_keys[] = {
        mgc::platform::input::Key::Up,
        mgc::platform::input::Key::Down,
        mgc::platform::input::Key::Left,
        mgc::platform::input::Key::Right,
        mgc::platform::input::Key::Enter,
        mgc::platform::input::Key::Cancel,
        mgc::platform::input::Key::Menu,
        mgc::platform::input::Key::Option,
        mgc::platform::input::Key::Home,
        mgc::platform::input::Key::End,
        mgc::platform::input::Key::Next,
        mgc::platform::input::Key::Previous,
        mgc::platform::input::Key::Control
    };

    for ( auto key : all_keys ) {
        if ( button.is_pressed(key) ) {
            return true;
        }
    }
    return false;
}


}// namespace input
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_INPUT_IBUTTON_HPP*/
