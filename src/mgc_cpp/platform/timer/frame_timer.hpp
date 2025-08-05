/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PLATFORM_TIMER_FRAME_TIMER_HPP
#define MGC_PLATFORM_TIMER_FRAME_TIMER_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace platform {
namespace timer {

template <typename BaseTimer>
struct FrameTimer {

    using timestamp_t = typename BaseTimer::timestamp_t;

    FrameTimer() { reset(); }

    void reset() {
        last_base_time_ms_ = BaseTimer::now_ms();
        frame_time_ms_ = 0;
    }

    void tick() {
        auto now = BaseTimer::now_ms();
        auto delta = now - last_base_time_ms_;
        frame_time_ms_ += delta;
        last_base_time_ms_ = now;
    }

    timestamp_t now_ms() const {
        return frame_time_ms_;
    }

    uint32_t now_ms32() const {
        return static_cast<uint32_t>(frame_time_ms_);
    }


private:
    timestamp_t frame_time_ms_{0};
    timestamp_t last_base_time_ms_{0};
};

}// namespace timer
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_TIMER_FRAME_TIMER_HPP*/
