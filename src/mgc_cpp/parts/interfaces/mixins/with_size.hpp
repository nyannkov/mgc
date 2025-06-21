/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_SIZE_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_SIZE_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/types/size.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithSize {

    void set_size(uint16_t width, uint16_t height) {
        static_cast<Derived*>(this)->set_size_impl(width, height);
    }

    void set_size(mgc::parts::types::Size size) {
        auto s = size.to_c();
        set_size(s.width, s.height);
    }

    mgc::parts::types::Size size() const {
        return static_cast<const Derived*>(this)->size_impl();
    }

};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_SIZE_HPP*/
