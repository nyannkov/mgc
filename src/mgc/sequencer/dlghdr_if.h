/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DLGHDR_IF_H
#define MGC_DLGHDR_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "dlgflow.h"

typedef struct mgc_dlghdr_if {
    void (*init)(void);
    void (*display_text)(const char * text, enum mgc_dlg_wait_type wait_type);
    void (*close_text)(void);
    bool (*is_display_text_finished)(void);
    bool (*is_response_yes)(void);
} mgc_dlghdr_if_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DLGHDRL_IF_H*/
