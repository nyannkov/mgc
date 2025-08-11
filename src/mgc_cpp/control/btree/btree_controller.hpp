/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CONTROL_BTREE_BTREE_CONTROLLER_HPP
#define MGC_CONTROL_BTREE_BTREE_CONTROLLER_HPP

#include <type_traits>
#include "btree_listener.hpp"
#include "btree_duration.hpp"
#include "mgc/sequencer/btctrl.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/parts/assets/btree.hpp"
#include "mgc_cpp/platform/timer/timer.hpp"

namespace mgc {
namespace control {
namespace btree {

template <typename FrameTimerT>
struct BTreeController : mgc::features::Resettable {
    using timestamp_t = typename FrameTimerT::timestamp_t;
    using DurationT = BTreeDuration<timestamp_t>;

    BTreeController(const FrameTimerT& timer) : timer_(timer) { reset(); }
    ~BTreeController() = default;
    BTreeController(const BTreeController&) = delete;
    BTreeController& operator=(const BTreeController&) = delete;
    BTreeController(BTreeController&&) = default;
    BTreeController& operator=(BTreeController&&) = default;

    void bind_listener(IBTreeListener<BTreeController<FrameTimerT>>& listener) {
        listener_ = &listener;
    }

    void unbind_listener() {
        listener_ = nullptr;
    }
    
    void set_btree(const mgc::parts::assets::BTree& btree) {
        btctrl_set_btree(&btctrl_, &btree);
    }

    void proc(bool input_detected) {

        timer_now_ = timer_.now_ms();

        if (input_detected) {
            reset_input_timer(timer_now_);
        }

        update_duration(timer_now_);

        btctrl_proc(&btctrl_);
    }

    void proc_until_blocked(bool input_detected) {

        timer_now_ = timer_.now_ms();

        if (input_detected) {
            reset_input_timer(timer_now_);
        }

        update_duration(timer_now_);

        bool should_continue = true;
        while (should_continue) {
            btctrl_proc(&btctrl_);
            auto state = btctrl_get_state(&btctrl_);
            auto leaf_result = btctrl_get_last_leaf_result(&btctrl_);

            should_continue = 
                (state != MGC_BTCTRL_STATE_FINISHED) &&
                (leaf_result != MGC_BTREE_LEAF_RESULT_RUNNING);
        }
    }

    bool in_progress() const {
        auto state = btctrl_get_state(&btctrl_);
        
        return ( state == MGC_BTCTRL_STATE_IN_PROGRESS );
    }

    bool has_finished() const {
        auto state = btctrl_get_state(&btctrl_);
        return ( state == MGC_BTCTRL_STATE_FINISHED );
    }

    void reset_state() {
        btctrl_reset_state(&btctrl_);
        timer_now_ = timer_.now_ms();
        reset_btree_timer(timer_now_);
        // don't reset input timer
    }

    // [feature] Resettable
    void reset() override {

        btctrl_init(&btctrl_);
        bind_callbacks();
        listener_ = nullptr;

        timer_now_ = timer_.now_ms();
        reset_btree_timer(timer_now_);
        reset_input_timer(timer_now_);
        update_duration(timer_now_);
    }

private:
    mgc_btctrl_t btctrl_;
    const FrameTimerT& timer_;
    IBTreeListener<BTreeController<FrameTimerT>>* listener_;
    DurationT duration_;
    timestamp_t timer_now_;
    timestamp_t tree_start_time_;
    timestamp_t composite_entry_time_;
    timestamp_t leaf_entry_time_;
    timestamp_t last_input_time_;

    mgc_btctrl_callbacks_t callbacks_;

    void reset_btree_timer(timestamp_t now) {
        tree_start_time_ = now;
        composite_entry_time_ = now;
        leaf_entry_time_ = now;
    }

    void reset_input_timer(timestamp_t now) {
        last_input_time_ = now;
    }

    void update_duration(timestamp_t now) {
        duration_.tree_elapsed = now - tree_start_time_;
        duration_.composite_elapsed = now - composite_entry_time_;
        duration_.leaf_elapsed = now - leaf_entry_time_;
        duration_.input_idle_time = now - last_input_time_;
    }

    void bind_callbacks() {
        callbacks_ = mgc_btctrl_callbacks_t {
            this,
            on_proc_leaf_wrapper,
            on_enter_leaf_wrapper,
            on_exit_leaf_wrapper,
            on_enter_composite_wrapper,
            on_exit_composite_wrapper,
            on_tree_start_wrapper,
            on_tree_finish_wrapper
        };
        btctrl_set_callbacks(&btctrl_, &callbacks_);
    }

    static enum mgc_btree_leaf_result on_proc_leaf_wrapper(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context) {
        return static_cast<BTreeController*>(context)->on_proc_leaf(btctrl, leaf);
    }

    static void on_enter_leaf_wrapper(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context) {
        static_cast<BTreeController*>(context)->on_enter_leaf(btctrl, leaf);
    }

    static void on_exit_leaf_wrapper(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context) {
        static_cast<BTreeController*>(context)->on_exit_leaf(btctrl, leaf);
    }
    static void on_enter_composite_wrapper(mgc_btctrl_t* btctrl, void *context) {
        static_cast<BTreeController*>(context)->on_enter_composite(btctrl);
    }

    static void on_exit_composite_wrapper(mgc_btctrl_t* btctrl, void *context) {
        static_cast<BTreeController*>(context)->on_exit_composite(btctrl);
    }

    static void on_tree_start_wrapper(mgc_btctrl_t* btctrl, void *context) {
        static_cast<BTreeController*>(context)->on_tree_start(btctrl);
    }

    static void on_tree_finish_wrapper(mgc_btctrl_t* btctrl, void *context) {
        static_cast<BTreeController*>(context)->on_tree_finish(btctrl);
    }

    enum mgc_btree_leaf_result on_proc_leaf(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf) {

        if ( listener_ ) {
            std::string_view id = (leaf->id != nullptr) ? std::string_view(leaf->id) : std::string_view { };
            auto state = listener_->on_proc_leaf(id, duration_, btctrl->current->tag);
            switch ( state ) {
            case IBTreeListener<BTreeController<FrameTimerT>>::LeafResult::Running: return MGC_BTREE_LEAF_RESULT_RUNNING;
            case IBTreeListener<BTreeController<FrameTimerT>>::LeafResult::Success: return MGC_BTREE_LEAF_RESULT_SUCCESS;
            case IBTreeListener<BTreeController<FrameTimerT>>::LeafResult::Failure: return MGC_BTREE_LEAF_RESULT_FAILURE;
            default: return MGC_BTREE_LEAF_RESULT_FAILURE;
            }

        } else {
            return MGC_BTREE_LEAF_RESULT_FAILURE;
        }
    }

    void on_enter_leaf(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf) {
        leaf_entry_time_ = timer_now_;
        update_duration(timer_now_);
        if ( listener_ ) {
            std::string_view id = (leaf->id != nullptr) ? std::string_view(leaf->id) : std::string_view { };
            listener_->on_enter_leaf(id, btctrl->current->tag);
        }
    }

    void on_exit_leaf(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf) {
        if ( listener_ ) {
            std::string_view id = (leaf->id != nullptr) ? std::string_view(leaf->id) : std::string_view { };
            listener_->on_exit_leaf(id, btctrl->current->tag);
        }
    }

    void on_enter_composite(mgc_btctrl_t* btctrl) {
        composite_entry_time_ = timer_now_;
        update_duration(timer_now_);
        if ( listener_ ) {
            listener_->on_enter_composite(btctrl->current->tag);
        }
    }

    void on_exit_composite(mgc_btctrl_t* btctrl) {
        if ( listener_ ) {
            listener_->on_exit_composite(btctrl->current->tag);
        }
    }

    void on_tree_start(mgc_btctrl_t* btctrl) {
        tree_start_time_ = timer_now_;
        update_duration(timer_now_);
        if ( listener_ ) {
            listener_->on_tree_start(btctrl->current->tag);
        }
    }

    void on_tree_finish(mgc_btctrl_t* btctrl) {
        if ( listener_ ) {
            listener_->on_tree_finish(btctrl->current->tag);
        }
    }
};

}// namespace btree
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_BTREE_BTREE_CONTROLLER_HPP*/
