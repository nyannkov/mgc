/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_MARGIN_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_MARGIN_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/types/types.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithMargin {

    void set_margin(mgc::parts::types::Margin margin) {
        static_cast<Derived*>(this)->set_margin_impl(margin);
    }

    mgc::parts::types::Margin get_margin() const {
        return static_cast<const Derived*>(this)->get_margin_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_MARGIN_HPP*/
