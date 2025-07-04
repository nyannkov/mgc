/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CONTROL_BTREE_BTREE_DURATION_HPP
#define MGC_CONTROL_BTREE_BTREE_DURATION_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace control {
namespace btree {

template <typename TimestampT>
struct BTreeDuration {
    TimestampT tree_elapsed;
    TimestampT composite_elapsed;
    TimestampT leaf_elapsed;
    TimestampT input_idle_time;
};

}// namespace btree
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_BTREE_BTREE_DURATION_HPP*/
