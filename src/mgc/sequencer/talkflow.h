/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_TALKFLOW_H
#define MGC_TALKFLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "talkscript.h"

enum mgc_talkflow_state {
    MGC_TALKFLOW_STATE_INIT,
    MGC_TALKFLOW_STATE_SETUP,
    MGC_TALKFLOW_STATE_PROCESSING,
    MGC_TALKFLOW_STATE_FLOW_END,
};

enum mgc_talkflow_ui_state {
    MGC_TALKFLOW_UI_STATE_CONTINUE,
    MGC_TALKFLOW_UI_STATE_FINISHED,
};

typedef struct mgc_talkflow mgc_talkflow_t;

typedef struct mgc_talkflow_callbacks{
    void *context;
    void (*on_ui_setup_message)(
            mgc_talkflow_t *talkflow,
            mgc_node_idx_t tag,
            const char *message_string,
            void *context
    );
    void (*on_ui_setup_message_format)(
            mgc_talkflow_t *talkflow,
            mgc_node_idx_t tag,
            const char *format_string,
            void *context
    );
    void (*on_ui_setup_choice)(
            mgc_talkflow_t *talkflow,
            mgc_node_idx_t tag,
            const mgc_node_choice_item_t *items,
            size_t item_count,
            void *context
    );
    enum mgc_talkflow_ui_state (*on_ui_proc_message)(
            mgc_talkflow_t *talkflow,
            mgc_node_idx_t tag,
            const mgc_node_message_t *message,
            void *context
    );
    enum mgc_talkflow_ui_state (*on_ui_proc_choice)(
            mgc_talkflow_t *talkflow,
            mgc_node_idx_t tag,
            const mgc_node_choice_t *choice,
            void *context
    );
    bool (*on_decision)(
            mgc_talkflow_t *talkflow,
            mgc_node_idx_t tag,
            void *context
    );
    void (*on_flow_end)(
            mgc_talkflow_t *talkflow,
            mgc_node_idx_t tag,
            const mgc_talknode_t *node,
            void *context
    );
} mgc_talkflow_callbacks_t;

struct mgc_talkflow {
    const mgc_talkscript_t *talkscript;
    size_t current_node_idx;
    size_t current_item_idx;
    enum mgc_talkflow_state state;
    mgc_talkflow_callbacks_t callbacks;
};

void talkflow_init(mgc_talkflow_t *talkflow);
void talkflow_set_talkscript(
        mgc_talkflow_t *talkflow,
        const mgc_talkscript_t *talkscript
);
void talkflow_set_callbacks(
        mgc_talkflow_t *talkflow,
        const mgc_talkflow_callbacks_t *callbacks
);
void talkflow_begin(mgc_talkflow_t *talkflow, mgc_node_idx_t entry_label);
enum mgc_talkflow_state talkflow_proc(mgc_talkflow_t *talkflow);

enum mgc_talkflow_state talkflow_get_state(const mgc_talkflow_t *talkflow);
void talkflow_decide_choice(mgc_talkflow_t *talkflow, size_t item_tag);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_TALKFLOW_H*/
