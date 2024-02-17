/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DLGCTRL_H
#define MGC_DLGCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "dlghdr_if.h"
#include "dlgflow.h"

enum mgc_dlg_flow_state {
    MGC_DLG_FLOW_NOT_SET,
    MGC_DLG_FLOW_READY,
    MGC_DLG_FLOW_IN_PROGRESS,
    MGC_DLG_FLOW_SUSPEND,
    MGC_DLG_FLOW_END,
};

typedef struct mgc_dlgctrl {
    const mgc_dlg_flow_t *flow;
    size_t current_node_idx;
    enum mgc_dlg_flow_state flow_state;
    enum mgc_dlg_resp latest_resp;
    const mgc_dlghdr_if_t *dlghdr;
} mgc_dlgctrl_t;

void dlgctrl_init(mgc_dlgctrl_t *dlgctrl, const mgc_dlghdr_if_t *dlghdr);
void dlgctrl_set_flow(mgc_dlgctrl_t *dlgctrl, const mgc_dlg_flow_t *flow, uint16_t node_id);
void dlgctrl_clear_flow(mgc_dlgctrl_t *dlgctrl);
void dlgctrl_update_flow(mgc_dlgctrl_t *dlgctrl);
void dlgctrl_resume_flow(mgc_dlgctrl_t *dlgctrl);
enum mgc_dlg_flow_state dlgctrl_get_flow_state(const mgc_dlgctrl_t *dlgctrl);
uint16_t dlgctrl_get_flow_id(const mgc_dlgctrl_t *dlgctrl);
uint16_t dlgctrl_get_node_id(const mgc_dlgctrl_t *dlgctrl);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DLGCTRL_H*/
