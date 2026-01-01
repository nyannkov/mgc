#include <chrono>
#include "free_running_timer_port.h"

uint32_t free_running_timer_port__get_now_ms(void) {
    using namespace std::chrono;

    const auto now = steady_clock::now().time_since_epoch();
    const uint64_t ms =
        duration_cast<milliseconds>(now).count();

    return static_cast<uint32_t>(ms);
}

