/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "talkflow.h"


void talkflow_init(mgc_talkflow_t *talkflow) {
    if ( talkflow == NULL )  {
        MGC_WARN("Invalid handler");
        return;
    }
    talkflow->talkscript = NULL;
    talkflow->current_node_idx = 0;
    talkflow->current_item_idx = 0;
    talkflow->state = MGC_TALKFLOW_STATE_INIT;
    talkflow->callbacks.context = NULL;
    talkflow->callbacks.on_ui_setup_message = NULL;
    talkflow->callbacks.on_ui_setup_message_format = NULL;
    talkflow->callbacks.on_ui_setup_choice = NULL;
    talkflow->callbacks.on_ui_proc_message = NULL;
    talkflow->callbacks.on_ui_proc_choice = NULL;
    talkflow->callbacks.on_decision = NULL;
    talkflow->callbacks.on_flow_end = NULL;
}

void talkflow_set_talkscript(
        mgc_talkflow_t *talkflow,
        const mgc_talkscript_t *talkscript
) {
    if ( ( talkflow == NULL ) ||
         ( talkscript == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    talkflow->talkscript = talkscript;
}

void talkflow_set_callbacks(
        mgc_talkflow_t *talkflow,
        const mgc_talkflow_callbacks_t *callbacks
) {
    if ( ( talkflow == NULL ) ||
         ( callbacks == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    talkflow->callbacks.context = callbacks->context;
    talkflow->callbacks.on_ui_setup_message = callbacks->on_ui_setup_message;
    talkflow->callbacks.on_ui_setup_message_format = callbacks->on_ui_setup_message_format;
    talkflow->callbacks.on_ui_setup_choice = callbacks->on_ui_setup_choice;
    talkflow->callbacks.on_ui_proc_message = callbacks->on_ui_proc_message;
    talkflow->callbacks.on_ui_proc_choice = callbacks->on_ui_proc_choice;
    talkflow->callbacks.on_decision = callbacks->on_decision;
    talkflow->callbacks.on_flow_end = callbacks->on_flow_end;
}

void talkflow_begin(mgc_talkflow_t *talkflow, mgc_node_idx_t entry_label) {
    if ( ( talkflow == NULL ) ||
         ( talkflow->talkscript == NULL ) ||
         ( talkflow->talkscript->node_count <= entry_label )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    talkflow->current_node_idx = entry_label;
    talkflow->current_item_idx = 0;
    talkflow->state = MGC_TALKFLOW_STATE_SETUP;
}

static inline enum mgc_talkflow_ui_state node_message_proc(
        mgc_talkflow_t *talkflow,
        size_t current_index,
        const mgc_node_message_t *message
) {

    enum mgc_talkflow_ui_state ui_state = MGC_TALKFLOW_UI_STATE_CONTINUE;
    const mgc_talkflow_callbacks_t *callbacks = &talkflow->callbacks;

    if ( talkflow->state == MGC_TALKFLOW_STATE_SETUP ) {
        if ( message->format ) {
            if ( callbacks->on_ui_setup_message_format != NULL ) {
                callbacks->on_ui_setup_message_format(
                        talkflow,
                        current_index,
                        message->text,
                        callbacks->context
                );
            }
        } else {
            if ( callbacks->on_ui_setup_message != NULL ) {
                callbacks->on_ui_setup_message(
                        talkflow,
                        current_index,
                        message->text,
                        callbacks->context
                );
            }
        }
        talkflow->state = MGC_TALKFLOW_STATE_PROCESSING;

    } else if ( talkflow->state == MGC_TALKFLOW_STATE_PROCESSING ) {
        if ( callbacks->on_ui_proc_message != NULL ) {
            ui_state = callbacks->on_ui_proc_message(
                    talkflow,
                    current_index,
                    message,
                    callbacks->context
            );
        } else {
            ui_state = MGC_TALKFLOW_UI_STATE_FINISHED;
        }
    } else {
    }
    return ui_state;
}

static inline enum mgc_talkflow_ui_state node_choice_proc(
        mgc_talkflow_t *talkflow,
        size_t current_index,
        const mgc_node_choice_t *choice
) {

    enum mgc_talkflow_ui_state ui_state = MGC_TALKFLOW_UI_STATE_CONTINUE;
    const mgc_talkflow_callbacks_t *callbacks = &talkflow->callbacks;

    if ( talkflow->state == MGC_TALKFLOW_STATE_SETUP ) {
        if ( callbacks->on_ui_setup_choice != NULL ) {
            callbacks->on_ui_setup_choice(
                    talkflow,
                    current_index,
                    choice->items,
                    choice->item_count,
                    callbacks->context
            );
        }

        talkflow->state = MGC_TALKFLOW_STATE_PROCESSING;

    } else if ( talkflow->state == MGC_TALKFLOW_STATE_PROCESSING ) {
        if ( callbacks->on_ui_proc_choice != NULL ) {
            ui_state = callbacks->on_ui_proc_choice(
                    talkflow,
                    current_index,
                    choice,
                    callbacks->context
            );
        } else {
            ui_state = MGC_TALKFLOW_UI_STATE_FINISHED;
        }
    } else {
    }

    return ui_state;
}

enum mgc_talkflow_state talkflow_proc(mgc_talkflow_t *talkflow) {
    if ( ( talkflow == NULL ) ||
         ( talkflow->talkscript == NULL ) ||
         ( talkflow->talkscript->node_count <= talkflow->current_node_idx )
    ) {
        MGC_WARN("Invalid handler");
        return MGC_TALKFLOW_STATE_INIT;
    }

    enum mgc_talkflow_ui_state ui_state = MGC_TALKFLOW_UI_STATE_CONTINUE;
    size_t current_index = talkflow->current_node_idx;
    const mgc_talknode_t *current_node 
        = &talkflow->talkscript->nodes[current_index];
    const mgc_talkflow_callbacks_t *callbacks = &talkflow->callbacks;

    if ( current_node->type == MGC_TALKNODE_TYPE_MESSAGE ) {
        ui_state = node_message_proc(
                talkflow,
                current_index,
                current_node->content.message
        );
    } else if ( current_node->type == MGC_TALKNODE_TYPE_CHOICE ) {
        ui_state = node_choice_proc(
                talkflow,
                current_index,
                current_node->content.choice
        );

    } else if ( current_node->type == MGC_TALKNODE_TYPE_DECISION ) {
        ui_state = MGC_TALKFLOW_UI_STATE_FINISHED;
    } else {
        MGC_WARN("Invalid node type");
        return talkflow->state;
    }

    if ( ui_state == MGC_TALKFLOW_UI_STATE_FINISHED ) {
        size_t current_index = talkflow->current_node_idx;
        const mgc_talknode_t *current_node 
            = &talkflow->talkscript->nodes[current_index];

        if ( current_node->end ) {
            if ( callbacks->on_flow_end != NULL ) {
                callbacks->on_flow_end(
                        talkflow,
                        current_index,
                        current_node,
                        callbacks->context
                );
            }
            talkflow->state = MGC_TALKFLOW_STATE_FLOW_END;
        } else {
            switch ( current_node->type ) {
            case MGC_TALKNODE_TYPE_MESSAGE:
                talkflow->current_node_idx = current_node->content.message->next;
                break;

            case MGC_TALKNODE_TYPE_CHOICE:
                talkflow->current_node_idx = 
                    current_node->content.choice->items[talkflow->current_item_idx].next;
                break;
            case MGC_TALKNODE_TYPE_DECISION:
                if ( callbacks->on_decision != NULL ) {
                    bool result = callbacks->on_decision(
                            talkflow,
                            current_index,
                            callbacks->context
                    );
                    talkflow->current_node_idx = result ? 
                        current_node->content.decision->next_if_true :
                        current_node->content.decision->next_if_false;
                } else {
                    talkflow->current_node_idx = 
                        current_node->content.decision->next_if_false;
                }
            default:
                break;
            }
            talkflow->state = MGC_TALKFLOW_STATE_SETUP;
        }
    }

    return talkflow->state;
}

enum mgc_talkflow_state talkflow_get_state(const mgc_talkflow_t *talkflow) {
    if ( talkflow == NULL )  {
        MGC_WARN("Invalid handler");
        return MGC_TALKFLOW_STATE_INIT;
    }
    return talkflow->state;
}

void talkflow_reset_state(mgc_talkflow_t *talkflow) {
    if ( talkflow == NULL )  {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( talkflow->state == MGC_TALKFLOW_STATE_FLOW_END ) {
        talkflow->state = MGC_TALKFLOW_STATE_INIT;
    }
}

void talkflow_decide_choice(mgc_talkflow_t *talkflow, size_t item_tag) {
    if ( ( talkflow == NULL ) ||
         ( talkflow->talkscript == NULL ) ||
         ( talkflow->talkscript->node_count <= talkflow->current_node_idx )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    const mgc_talknode_t *current_node 
        = &talkflow->talkscript->nodes[talkflow->current_node_idx];

    if ( current_node->type != MGC_TALKNODE_TYPE_CHOICE ) {
        MGC_WARN("Current type is not choice");
        return;
    }

    if ( current_node->content.choice->item_count <= item_tag ) {
        MGC_WARN("The item is out of range");
        return;
    }

    talkflow->current_item_idx = item_tag;
}

