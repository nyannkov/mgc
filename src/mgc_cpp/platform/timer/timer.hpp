/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PLATFORM_TIMER_TIMER_HPP
#define MGC_PLATFORM_TIMER_TIMER_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace platform {
namespace timer {

template <typename Derived, typename TimestampT>
struct Timer {
    using timestamp_t = TimestampT;

    static timestamp_t now_ms() {
        return Derived::now_ms_impl();
    }
};

}// namespace timer
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_TIMER_TIMER_HPP*/
