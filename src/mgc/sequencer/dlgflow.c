/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "dlgflow.h"

static size_t get_node_idx_by_node_id(const mgc_dlgnode_t *node_array, size_t node_count, mgc_node_id_t node_id) {
    size_t idx;
    for ( idx = 0; idx < node_count; idx++ ) {
        if ( node_array[idx].id == node_id ) {
            break;
        }
    }
    return idx;
}


void dlgflow_init(mgc_dlgflow_t *dlgflow, const mgc_dlgflow_if_t *handler) {
    if ( ( dlgflow == NULL ) ||
         ( handler == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    dlgflow->node_array = NULL;
    dlgflow->node_count = 0;
    dlgflow->current_node_idx = 0;
    dlgflow->handler = handler;
    dlgflow->state = MGC_DLG_FLOW_STOP;
}

void dlgflow_set_node_array(mgc_dlgflow_t *dlgflow, const mgc_dlgnode_t *node_array, size_t node_count, mgc_node_id_t start_id) {
    size_t start_idx;
    if ( ( dlgflow == NULL ) ||
         ( node_array == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    dlgflow->node_array = node_array;
    dlgflow->node_count = node_count;
    start_idx = get_node_idx_by_node_id(node_array, node_count, start_id);
    if ( start_idx < node_count ) {
        dlgflow->current_node_idx = start_idx;
        dlgflow->state = MGC_DLG_FLOW_READY;
    } else {
        MGC_WARN("start_id is not found");
        dlgflow->current_node_idx = 0;
        dlgflow->state = MGC_DLG_FLOW_ERROR;
    }
}

bool dlgflow_run_node_proc(mgc_dlgflow_t *dlgflow) {
    const mgc_dlgnode_t *current_node;
    if ( ( dlgflow == NULL ) ||
         ( dlgflow->node_array == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    current_node = &dlgflow->node_array[dlgflow->current_node_idx];

    if ( dlgflow->state == MGC_DLG_FLOW_READY ) {
        switch ( current_node->type ) {
        case MGC_DLG_TYPE_TEXT:
            dlgflow->handler->show_dialoguebox(
                current_node->params.text,
                current_node->flags
            );
            dlgflow->state = MGC_DLG_FLOW_NODE_PROGRESSING;
            break;
        case MGC_DLG_TYPE_SELECT:
            dlgflow->handler->show_selectbox(
                current_node->params.items->item_array,
                current_node->params.items->item_count,
                current_node->flags
            );
            dlgflow->state = MGC_DLG_FLOW_NODE_PROGRESSING;
            break;
        default:
            MGC_WARN("Unknown type");
            dlgflow->state = MGC_DLG_FLOW_ERROR;
            break;
        }
    } else if ( dlgflow->state == MGC_DLG_FLOW_NODE_PROGRESSING ) {
        int32_t result = 0;
        switch ( current_node->type ) {
        case MGC_DLG_TYPE_TEXT:
            if ( dlgflow->handler->proc_dialoguebox(&result) ) {
                dlgflow->result = result;
                if ( current_node->terminal == true ) {
                    dlgflow->handler->close_dialoguebox();
                }
                dlgflow->state = MGC_DLG_FLOW_NODE_COMPLETED;
            }
            break;
        case MGC_DLG_TYPE_SELECT:
            if ( dlgflow->handler->proc_selectbox(&result) ) {
                dlgflow->result = result;
                dlgflow->handler->close_selectbox();
                dlgflow->state = MGC_DLG_FLOW_NODE_COMPLETED;
            }
            break;
        default:
            MGC_WARN("Unknown type");
            dlgflow->state = MGC_DLG_FLOW_ERROR;
            break;
        }
    } else {
    }

    return ( dlgflow->state == MGC_DLG_FLOW_NODE_PROGRESSING );
}

bool dlgflow_is_flow_terminated(const mgc_dlgflow_t *dlgflow) {
    const mgc_dlgnode_t *current_node;
    if ( ( dlgflow == NULL ) ||
         ( dlgflow->node_array == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    current_node = &dlgflow->node_array[dlgflow->current_node_idx];
    if ( ( current_node->terminal == true ) &&
         ( dlgflow->state == MGC_DLG_FLOW_NODE_COMPLETED )
    ) {
        return true;
    } else {
        return false;
    }
}

enum mgc_dlgflow_state dlgflow_get_state(const mgc_dlgflow_t *dlgflow) {
    if ( dlgflow == NULL ) {
        MGC_WARN("Invalid handler");
        return MGC_DLG_FLOW_ERROR;
    }
    return dlgflow->state;
}


mgc_node_id_t dlgflow_get_current_node_id(const mgc_dlgflow_t *dlgflow) {
    const mgc_dlgnode_t *current_node;
    if ( ( dlgflow == NULL ) ||
         ( dlgflow->node_array == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    current_node = &dlgflow->node_array[dlgflow->current_node_idx];
    return current_node->id;
}

void dlgflow_switch_to_next_node(mgc_dlgflow_t *dlgflow) {
    const mgc_dlgnode_t *current_node;
    if ( ( dlgflow == NULL ) ||
         ( dlgflow->node_array == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( dlgflow->state != MGC_DLG_FLOW_NODE_COMPLETED ) {
        MGC_WARN("Node is not completed");
        return;
    }
    
    current_node = &dlgflow->node_array[dlgflow->current_node_idx];

    if ( current_node->cb_before_switch_node != NULL ) {
        mgc_node_id_t next_node_id;
        size_t idx;
        next_node_id = current_node->cb_before_switch_node(dlgflow);
        idx = get_node_idx_by_node_id(dlgflow->node_array, dlgflow->node_count, next_node_id);
        if ( idx < dlgflow->node_count ) {
            dlgflow->current_node_idx = idx;
            dlgflow->state = MGC_DLG_FLOW_READY;
        } else {
            dlgflow->state = MGC_DLG_FLOW_ERROR;
        }
    } else {
        if ( dlgflow->current_node_idx < (dlgflow->node_count - 1) ) {
            dlgflow->current_node_idx++;
            dlgflow->state = MGC_DLG_FLOW_READY;
        } else {
            dlgflow->state = MGC_DLG_FLOW_ERROR;
        }
    }
}

int32_t dlgflow_get_node_result(const mgc_dlgflow_t *dlgflow) {
    if ( dlgflow == NULL ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    return dlgflow->result;
}

void dlgflow_clear_state(mgc_dlgflow_t *dlgflow) {
    if ( dlgflow == NULL ) {
        MGC_WARN("Invalid handler");
    }
    dlgflow->state = MGC_DLG_FLOW_STOP;
}
