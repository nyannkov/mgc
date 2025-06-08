/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_TYPING_EFFECT_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_TYPING_EFFECT_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithTypingEffect {

    void set_typing_speed(uint32_t chars_per_frame) {
        static_cast<Derived*>(this)->set_typing_speed_impl(chars_per_frame);
    }

    uint32_t get_typing_speed() const {
        return static_cast<const Derived*>(this)->get_typing_speed_impl();
    }

    void advance_typing() {
        static_cast<Derived*>(this)->advance_typing_impl();
    }

    bool is_typing_complete() const {
        return static_cast<const Derived*>(this)->is_typing_complete_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_TYPING_EFFECT*/
