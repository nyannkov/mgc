/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DLGFLOW_IF_H
#define MGC_DLGFLOW_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef struct mgc_dlgflow_if {
    void (*show_dialoguebox)(const char *text, uint32_t flags);
    bool (*proc_dialoguebox)(int32_t *result);
    void (*close_dialoguebox)(void);
    void (*show_selectbox)(const char **item_array, size_t item_count, uint32_t flags);
    bool (*proc_selectbox)(int32_t *result);
    void (*close_selectbox)(void);
} mgc_dlgflow_if_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DLGFLOW_IF_H*/
