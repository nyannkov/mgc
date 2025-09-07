#ifndef MGC_UTILS_STOPWATCH_HPP
#define MGC_UTILS_STOPWATCH_HPP

#include "platform_conf.hpp"

namespace app {

struct StopWatch {
    explicit StopWatch(const FrameTimerT& frame_timer)
        : frame_timer_(frame_timer) { reset(); }
    ~StopWatch() = default;

    void reset() {
        base_ms_ = 0;
        elapsed_ms_ = 0;
        running_ = false;
    }

    void start() {
        base_ms_ = frame_timer_.now_ms();
        running_ = true;
    }

    void stop() {
        running_ = false;
    }

    FrameTimerT::timestamp_t elapsed_ms() {
        if ( running_ ) {
            elapsed_ms_ = frame_timer_.now_ms() - base_ms_;
        }
        return elapsed_ms_;
    }

    void restart() {
        base_ms_ = frame_timer_.now_ms();
        elapsed_ms_ = 0;
        running_ = true;
    }
    
private:
    const FrameTimerT& frame_timer_;
    FrameTimerT::timestamp_t base_ms_;
    FrameTimerT::timestamp_t elapsed_ms_;
    bool running_;
};

}// namespace app

#endif/*MGC_UTILS_STOPWATCH_HPP*/

