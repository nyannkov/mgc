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
    bool just_pressed(mgc::platform::input::Key key) const {
        return ( is_pressed(key) && !was_pressed(key) );
    }
    bool just_released(mgc::platform::input::Key key) const {
        return ( !is_pressed(key) && was_pressed(key) );
    }
};

}// namespace input
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_INPUT_IBUTTON_HPP*/
