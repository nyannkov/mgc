/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CONTROL_TALKFLOW_ITALKFLOW_LISTENER_HPP
#define MGC_CONTROL_TALKFLOW_ITALKFLOW_LISTENER_HPP

#include "mgc/sequencer/talkflow.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/geometry/point.hpp"

namespace mgc {
namespace control {
namespace talkflow {

struct SelectboxConfig {
    mgc::geometry::Point point;
    mgc::parts::types::Size size;
    const char *cursor_symbol;
    mgc::geometry::Point cursor_offset;
};

struct DialogueboxConfig {
    mgc::geometry::Point point;
    mgc::parts::types::Size size;
    uint32_t scroll_speed;
    uint32_t scroll_threshold_line;
    uint32_t line_spacing;
    uint32_t typing_speed;
};

struct ITalkflowListener {
    virtual ~ITalkflowListener() = default;
    virtual const char *on_get_message_format(mgc_node_idx_t tag, const char *format_string) {
        // NOTE:
        // The returned string must remain valid until the next on_get_message_format()
        // or on_flow_end(). No copy is made - insure the buffer stays alive and unmodified.
        //
        return "";
    }
    virtual bool on_start_message(mgc_node_idx_t tag, DialogueboxConfig& dialoguebox_config) { return false; }
    virtual bool on_start_choice(mgc_node_idx_t tag, SelectboxConfig& selectbox_config) { return false; }
    virtual void on_message_done(mgc_node_idx_t tag) {}
    virtual void on_choice_done(mgc_node_idx_t tag, size_t item_tag, int32_t value) {}
    virtual bool on_decision(mgc_node_idx_t tag) { return true; }
    virtual void on_flow_end(mgc_node_idx_t tag) {}
};

}// namespace talkflow
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_TALKFLOW_ITALKFLOW_LISTENER_HPP*/
