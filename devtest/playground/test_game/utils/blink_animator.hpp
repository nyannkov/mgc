#ifndef MGC_UTILS_BLINK_ANIMATOR_HPP
#define MGC_UTILS_BLINK_ANIMATOR_HPP

#include "mgc_cpp/mgc.hpp"
#include "platform_conf.hpp"

namespace app {

enum class BlinkAnimatorState {
    Stop,
    InProgress,
    Done
};

struct BlinkAnimator {

    explicit BlinkAnimator(const FrameTimerT& frame_timer)
        : sw_(frame_timer),
          state_(BlinkAnimatorState::Stop),
          target_(nullptr),
          blink_interval_ms_(200),
          blink_count_max_(5),
          blink_count_(0) { }
    ~BlinkAnimator() = default;

    BlinkAnimatorState state() const { return state_; }

    void set_target(mgc::features::Visible& target) {
        target_ = &target; 
    }

    void set_blink_interval(FrameTimerT::timestamp_t interval_ms) {
        blink_interval_ms_ = interval_ms;
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
            if ( sw_.elapsed_ms() >= blink_interval_ms_ ) {
                sw_.restart();
                target_->set_visible( !(target_->is_visible()) );
                blink_count_++;
                if ( blink_count_ >= blink_count_max_ ) {
                    state_ = BlinkAnimatorState::Done;
                    target_->set_visible(false);
                }
            }
        }
    }

private:
    StopWatch sw_;
    BlinkAnimatorState state_;
    FrameTimerT::timestamp_t blink_interval_ms_;
    size_t blink_count_max_;
    size_t blink_count_;
    mgc::features::Visible* target_;
};


}// namespace app

#endif/*MGC_UTILS_BLINK_ANIMATOR_HPP*/

