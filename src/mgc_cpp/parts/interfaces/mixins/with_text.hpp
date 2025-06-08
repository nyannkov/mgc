/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_TEXT_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_TEXT_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithText {

    void set_text(const char *text) {
        static_cast<Derived*>(this)->set_text_impl(text);
    }

    const char *get_text() const {
        return static_cast<const Derived*>(this)->get_text_impl();
    }

    void clear_text() {
        static_cast<Derived*>(this)->clear_text_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_TEXT*/
