/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_PADDING_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_PADDING_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/types/types.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithPadding {

    void set_padding(mgc::parts::types::Padding padding) {
        static_cast<Derived*>(this)->set_padding_impl(padding);
    }

    mgc::parts::types::Padding padding() const {
        return static_cast<const Derived*>(this)->padding_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_PADDING_HPP*/
