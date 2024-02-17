/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "dlgctrl.h"

static inline bool check_dlg_cond(enum mgc_dlg_cond cond, enum mgc_dlg_resp latest_resp) {
    if ( ( cond == MGC_DLG_TRUE ) ||
         ( ( cond == MGC_DLG_IF_YES ) && ( latest_resp == MGC_DLG_RESP_YES ) ) ||
         ( ( cond == MGC_DLG_IF_NO  ) && ( latest_resp == MGC_DLG_RESP_NO  ) )
    ) {
        return true;
    } else {
        return false;
    }
}

static const mgc_dlg_node_t *get_current_node(mgc_dlgctrl_t *dlgctrl) {
    for ( size_t idx = dlgctrl->current_node_idx; idx < dlgctrl->flow->node_count; idx++ ) {
        const mgc_dlg_node_t *node = &dlgctrl->flow->node_array[idx];
        if ( check_dlg_cond(node->cond, dlgctrl->latest_resp) == true ) {
            dlgctrl->current_node_idx = idx;
            return node;
        }
    }
    return NULL;
}

static bool get_node_array_index_by_node_id(const mgc_dlg_flow_t *flow, uint16_t node_id, size_t *index) {
    for ( size_t idx = 0; idx < flow->node_count; idx++ ) {
        if ( flow->node_array[idx].node_id == node_id ) {
            *index = idx;
            return true;
        }
    }
    return false;
}

void dlgctrl_init(mgc_dlgctrl_t *dlgctrl, const mgc_dlghdr_if_t *dlghdr) {
    if ( ( dlgctrl == NULL )
      || ( dlghdr == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    dlgctrl->flow = NULL;
    dlgctrl->current_node_idx = 0;
    dlgctrl->dlghdr = dlghdr;
    dlgctrl->latest_resp = MGC_DLG_RESP_NO;
    dlgctrl->flow_state = MGC_DLG_FLOW_NOT_SET;
}

void dlgctrl_set_flow(mgc_dlgctrl_t *dlgctrl, const mgc_dlg_flow_t *flow, uint16_t node_id) {
    size_t index;
    if ( ( dlgctrl == NULL ) ||
         ( flow == NULL ) ||
         ( flow->node_array == NULL ) ||
         ( flow->node_count == 0 )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( get_node_array_index_by_node_id(flow, node_id, &index) == true ) {
        dlgctrl->current_node_idx = index;
    } else {
        MGC_WARN("Not found");
        return;
    }
    dlgctrl->flow = flow;
    dlgctrl->dlghdr->init();
    dlgctrl->flow_state = MGC_DLG_FLOW_READY;
}

void dlgctrl_update_flow(mgc_dlgctrl_t *dlgctrl) {
    const mgc_dlg_node_t *node;
    if ( ( dlgctrl == NULL ) ||
         ( dlgctrl->dlghdr == NULL ) ||
         ( dlgctrl->flow == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    node = get_current_node(dlgctrl);
    if ( node == NULL ) {
        MGC_WARN("flow is over");
        dlgctrl->flow_state = MGC_DLG_FLOW_END;
        return;
    }

    if ( dlgctrl->flow_state == MGC_DLG_FLOW_READY ) {
        dlgctrl->dlghdr->display_text(node->text, node->wait_type);
        dlgctrl->flow_state = MGC_DLG_FLOW_IN_PROGRESS;

    } else if ( dlgctrl->flow_state == MGC_DLG_FLOW_IN_PROGRESS ) {
        if ( dlgctrl->dlghdr->is_display_text_finished() == true ) {
            if ( node->wait_type == MGC_DLG_ON_RECEIVE_YESNO ) {
                dlgctrl->latest_resp = ( dlgctrl->dlghdr->is_response_yes() == true )
                                     ? MGC_DLG_RESP_YES
                                     : MGC_DLG_RESP_NO;
            }
            switch ( node->next_ope ) {
            case MGC_DLG_STEP:
                if ( dlgctrl->current_node_idx < (dlgctrl->flow->node_count-1) ) {
                    dlgctrl->current_node_idx += 1;
                    dlgctrl->flow_state = MGC_DLG_FLOW_READY;
                } else {
                    MGC_WARN("flow is over");
                    dlgctrl->flow_state = MGC_DLG_FLOW_END;
                }
                break;

            case MGC_DLG_JUMP:
            {
                size_t index;
                if ( get_node_array_index_by_node_id(dlgctrl->flow, node->jump_node_id, &index) == true ) {
                    dlgctrl->current_node_idx = index;
                    dlgctrl->flow_state = MGC_DLG_FLOW_READY;
                } else {
                    MGC_WARN("jump_node_id is invalid");
                    dlgctrl->flow_state = MGC_DLG_FLOW_END;
                }
                break;
            }

            case MGC_DLG_SUSPEND:
                dlgctrl->flow_state = MGC_DLG_FLOW_SUSPEND;
                break;

            case MGC_DLG_EXIT:
                dlgctrl->dlghdr->close_text();
                dlgctrl->flow_state = MGC_DLG_FLOW_END;
                break;

            default:
                MGC_WARN("Unexpected next_ope");
                dlgctrl->flow_state = MGC_DLG_FLOW_END;
                break;
            }
        }
    } else {
    }
}

void dlgctrl_resume_flow(mgc_dlgctrl_t *dlgctrl) {
    if ( dlgctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( dlgctrl->flow_state == MGC_DLG_FLOW_SUSPEND ) {
        if ( dlgctrl->current_node_idx < (dlgctrl->flow->node_count-1) ) {
            dlgctrl->current_node_idx += 1;
            dlgctrl->flow_state = MGC_DLG_FLOW_READY;
        } else {
            MGC_WARN("flow is over");
            dlgctrl->flow_state = MGC_DLG_FLOW_END;
        }
    }
}

enum mgc_dlg_flow_state dlgctrl_get_flow_state(const mgc_dlgctrl_t *dlgctrl) {
    if ( dlgctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return MGC_DLG_FLOW_END;
    }
    return dlgctrl->flow_state;
}

uint16_t dlgctrl_get_flow_id(const mgc_dlgctrl_t *dlgctrl) {
    if ( ( dlgctrl == NULL ) ||
         ( dlgctrl->flow == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    return dlgctrl->flow->flow_id;
}

uint16_t dlgctrl_get_node_id(const mgc_dlgctrl_t *dlgctrl) {
    if ( ( dlgctrl == NULL ) ||
         ( dlgctrl->flow == NULL ) ||
         ( dlgctrl->flow->node_array == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    return dlgctrl->flow->node_array[dlgctrl->current_node_idx].node_id;
}

void dlgctrl_clear_flow(mgc_dlgctrl_t *dlgctrl) {
    if ( dlgctrl == NULL ) {
        return;
    }
    dlgctrl->flow = NULL;
    dlgctrl->flow_state = MGC_DLG_FLOW_NOT_SET;
}
