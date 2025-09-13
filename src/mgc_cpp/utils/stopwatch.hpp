/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_UTILS_STOPWATCH_HPP
#define MGC_UTILS_STOPWATCH_HPP

namespace mgc {
namespace utils {

///  Simple stopwatch for measuring elapsed time
///  FrameTimerT Expected to be a derived type of mgc::platform::timer::FrameTimer<T>.
template <typename FrameTimerT>
struct Stopwatch {
    using timestamp_t = typename FrameTimerT::timestamp_t;

    explicit Stopwatch(const FrameTimerT& frame_timer)
        : frame_timer_(frame_timer) { reset(); }

    ~Stopwatch() = default;

    void reset() {
        base_ms_ = 0;
        elapsed_ms_ = 0;
        running_ = false;
    }

    void start() {
        base_ms_ = frame_timer_.now_ms();
        running_ = true;
    }

    void stop() { running_ = false; }

    /// Get the current elapsed time in milliseconds.
    /// If running, updates the cached elapsed time.
    timestamp_t elapsed_ms() {
        if (running_) {
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
    timestamp_t base_ms_ = 0;
    timestamp_t elapsed_ms_ = 0;
    bool running_ = false;
};

} // namespace utils
} // namespace mgc

#endif // MGC_UTILS_STOPWATCH_HPP

