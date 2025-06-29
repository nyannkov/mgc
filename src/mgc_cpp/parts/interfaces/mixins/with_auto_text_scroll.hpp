/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_AUTO_TEXT_SCROLL_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_AUTO_TEXT_SCROLL_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithAutoTextScroll {

    void set_scroll_speed(uint32_t speed) {
        static_cast<Derived*>(this)->set_scroll_speed_impl(speed);
    }

    uint32_t scroll_speed() const {
        return static_cast<const Derived*>(this)->scroll_speed_impl();
    }

    void set_scroll_threshold_line(uint32_t line) {
        static_cast<Derived*>(this)->set_scroll_threshold_line_impl(line);
    }

    uint32_t scroll_threshold_line() const {
        return static_cast<const Derived*>(this)->scroll_threshold_line_impl();
    }

    void set_line_spacing(uint32_t line_spacing) {
        static_cast<Derived*>(this)->set_line_spacing_impl(line_spacing);
    }

    uint32_t line_spacing() const {
        return static_cast<const Derived*>(this)->line_spacing_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_AUTO_TEXT_SCROLL_HPP*/
