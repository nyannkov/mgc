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

enum mgc_dlg_resp {
    MGC_DLG_RESP_NO,
    MGC_DLG_RESP_YES,
};

enum mgc_dlg_cond {
    MGC_DLG_TRUE,
    MGC_DLG_IF_YES,
    MGC_DLG_IF_NO,
};

enum mgc_dlg_wait_type {
    MGC_DLG_ON_CURSOR_END,
    MGC_DLG_ON_BUTTON_PRESSED,
    MGC_DLG_ON_RECEIVE_YESNO,
};

enum mgc_dlg_next_ope {
    MGC_DLG_STEP,
    MGC_DLG_JUMP,
    MGC_DLG_SUSPEND,
    MGC_DLG_EXIT,
};

typedef struct mgc_dlg_node {
    uint16_t node_id;
    enum mgc_dlg_cond cond;
    const char * text;
    enum mgc_dlg_wait_type wait_type;
    enum mgc_dlg_next_ope next_ope;
    uint16_t jump_node_id;
} mgc_dlg_node_t;

typedef struct mgc_dlg_flow {
    uint16_t flow_id;
    const mgc_dlg_node_t *node_array;
    size_t node_count;
} mgc_dlg_flow_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DLGFLOW_H*/
