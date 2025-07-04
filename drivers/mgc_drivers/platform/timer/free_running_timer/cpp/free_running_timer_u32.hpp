/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PLATFORM_TIMER_FREE_RUNNING_TIMER_U32_HPP
#define MGC_PLATFORM_TIMER_FREE_RUNNING_TIMER_U32_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/platform/timer/timer.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/free_running_timer.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace timer {

struct FreeRunningTimerU32 : mgc::platform::timer::Timer<FreeRunningTimerU32, uint32_t> {

    static timestamp_t now_ms_impl() {
        return free_running_timer_get_now_ms();
    }
};

}// namespace timer
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_PLATFORM_TIMER_FREE_RUNNING_TIMER_U32_HPP*/
