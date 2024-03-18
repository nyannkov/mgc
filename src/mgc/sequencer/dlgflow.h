/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DLGFLOW_H
#define MGC_DLGFLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "dlgnode.h"
#include "dlgflow_if.h"

enum mgc_dlgflow_state {
    MGC_DLG_FLOW_STOP,
    MGC_DLG_FLOW_READY,
    MGC_DLG_FLOW_ERROR,
    MGC_DLG_FLOW_NODE_PROGRESSING,
    MGC_DLG_FLOW_NODE_COMPLETED
};

typedef struct mgc_dlgflow {
    const mgc_dlgnode_t *node_array;
    size_t node_count;
    size_t current_node_idx;
    int32_t result;
    enum mgc_dlgflow_state state;
    const mgc_dlgflow_if_t *handler;
} mgc_dlgflow_t;

void dlgflow_init(mgc_dlgflow_t *dlgflow, const mgc_dlgflow_if_t *handler);
void dlgflow_set_node_array(mgc_dlgflow_t *dlgflow, const mgc_dlgnode_t *node_array, size_t node_count, mgc_node_id_t start_id);
bool dlgflow_run_node_proc(mgc_dlgflow_t *dlgflow);
void dlgflow_switch_to_next_node(mgc_dlgflow_t *dlgflow);
void dlgflow_clear_state(mgc_dlgflow_t *dlgflow);

bool dlgflow_is_flow_terminated(const mgc_dlgflow_t *dlgflow);
enum mgc_dlgflow_state dlgflow_get_state(const mgc_dlgflow_t *dlgflow);
mgc_node_id_t dlgflow_get_current_node_id(const mgc_dlgflow_t *dlgflow);
int32_t dlgflow_get_node_result(const mgc_dlgflow_t *dlgflow);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DLGFLOW_H*/
