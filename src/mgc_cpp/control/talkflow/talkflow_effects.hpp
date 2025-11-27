/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CONTROL_TALKFLOW_ITALKFLOW_EFFECTS_HPP
#define MGC_CONTROL_TALKFLOW_ITALKFLOW_EFFECTS_HPP

#include "mgc/sequencer/talkflow.h"
#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace control {
namespace talkflow {

struct ITalkflowEffects {
    virtual ~ITalkflowEffects() = default;

    virtual void play_typing_sound(mgc_node_idx_t tag, bool is_scrolling) {}
    virtual void play_select_move_sound(mgc_node_idx_t tag) {}
    virtual void play_choice_confirm_sound(mgc_node_idx_t tag) {}
};

}// namespace talkflow
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_TALKFLOW_ITALKFLOW_EFFECTS_HPP*/
