/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_TRIM_REGION_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_TRIM_REGION_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/types/types.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithTrimRegion {

    void set_trim_region(mgc::parts::types::TrimRegion region) {
        static_cast<Derived*>(this)->set_trim_region_impl(region);
    }

    mgc::parts::types::TrimRegion trim_region() const {
        return static_cast<const Derived*>(this)->trim_region_impl();
    }

    void clear_trim_region() {
        static_cast<Derived*>(this)->clear_trim_region_impl();
    }

    bool has_trim_region() {
        return static_cast<Derived*>(this)->has_trim_region();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_TRIM_REGION_HPP*/
