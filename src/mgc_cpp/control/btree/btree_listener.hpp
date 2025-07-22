/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CONTROL_BTREE_IBTREE_LISTENER_HPP
#define MGC_CONTROL_BTREE_IBTREE_LISTENER_HPP

#include <string_view>
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/platform/timer/timer.hpp"
#include "mgc/sequencer/btree.h"
#include "btree_duration.hpp"

namespace mgc {
namespace control {
namespace btree {

template <typename BTreeControllerT>
struct IBTreeListener {

    using DurationT = typename BTreeControllerT::DurationT;

    enum class LeafResult {
        Running,
        Success,
        Failure
    };
    virtual ~IBTreeListener() = default;
    virtual LeafResult on_proc_leaf(std::string_view id, const DurationT& duration, mgc_btree_tag_t tag) = 0;

    virtual void on_enter_leaf(std::string_view id, mgc_btree_tag_t tag) { }
    virtual void on_exit_leaf(std::string_view id, mgc_btree_tag_t tag) { }
    virtual void on_enter_composite(mgc_btree_tag_t tag) { }
    virtual void on_exit_composite(mgc_btree_tag_t tag) { }
    virtual void on_tree_start(mgc_btree_tag_t tag) { }
    virtual void on_tree_finish(mgc_btree_tag_t tag) { }
};

}// namespace btree
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_BTREE_IBTREE_LISTENER_HPP*/
