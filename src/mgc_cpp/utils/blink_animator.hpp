/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_UTILS_BLINK_ANIMATOR_HPP
#define MGC_UTILS_BLINK_ANIMATOR_HPP

#include "mgc_cpp/features/visible.hpp"

namespace mgc {
namespace utils {

enum class BlinkAnimatorState {
    Stop,
    InProgress,
    Done
};

enum class BlinkEndState {
    Visible,
    Hidden
};

///  Simple stopwatch for measuring elapsed time
///  FrameTimerT Expected to be a derived type of mgc::platform::timer::FrameTimer<T>.
template <typename FrameTimerT>
struct BlinkAnimator {
    using timestamp_t = typename FrameTimerT::timestamp_t;

    explicit BlinkAnimator(const FrameTimerT& frame_timer)
        : sw_(frame_timer),
          state_(BlinkAnimatorState::Stop),
          end_state_(BlinkEndState::Hidden),
          target_(nullptr),
          blink_half_period_ms_(200),
          blink_count_max_(5),
          blink_count_(0) { }
    ~BlinkAnimator() = default;

    BlinkAnimatorState state() const { return state_; }

    BlinkEndState end_state() const { return end_state; }

    void set_target(mgc::features::Visible& target) {
        target_ = &target; 
    }

    void set_blink_half_period(timestamp_t half_period_ms) {
        blink_half_period_ms_ = half_period_ms;
    }

    timestamp_t blink_half_period() const { return blink_half_period_ms_; }

    void set_blink_count_max(size_t count_max) {
        blink_count_max_ = count_max;
    }

    size_t blink_count_max() const { return blink_count_max_; }

    size_t blink_count() const { return blink_count_; }

    void set_end_state(BlinkEndState end_state) {
        end_state_ = end_state;
    }

    void start() {
        if ( target_ ) {
            state_ = BlinkAnimatorState::InProgress;
            blink_count_ = 0;
            sw_.reset();
            sw_.start();
        } else {
            state_ = BlinkAnimatorState::Stop;
        }
    }

    void clear() { state_ = BlinkAnimatorState::Stop; }

    void update() {
        if ( !target_ ) return;
        if ( state_ == BlinkAnimatorState::InProgress ) {
            if ( sw_.elapsed_ms() >= blink_half_period_ms_ ) {
                sw_.restart();
                target_->set_visible( !(target_->is_visible()) );
                blink_count_++;
                if ( blink_count_ >= blink_count_max_ ) {
                    state_ = BlinkAnimatorState::Done;
                    if ( end_state_ == BlinkEndState::Visible ) {
                        target_->set_visible(true);
                    } else {
                        target_->set_visible(false);
                    }
                }
            }
        }
    }

private:
    Stopwatch<FrameTimerT> sw_;
    BlinkAnimatorState state_;
    BlinkEndState end_state_;
    timestamp_t blink_half_period_ms_;
    size_t blink_count_max_;
    size_t blink_count_;
    mgc::features::Visible* target_;
};

} // namespace utils
} // namespace mgc

#endif// MGC_UTILS_BLINK_ANIMATOR_HPP

