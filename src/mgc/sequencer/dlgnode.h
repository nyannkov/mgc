/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_DLGNODE_H
#define MGC_DLGNODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

struct mgc_dlgflow;
typedef uint32_t mgc_node_id_t;

enum mgc_dlgnode_type {
    MGC_DLG_TYPE_TEXT,
    MGC_DLG_TYPE_SELECT
};

typedef struct mgc_dlg_items {
    const char **item_array;
    size_t item_count;
} mgc_dlg_items_t;

typedef struct mgc_dlgnode {
    mgc_node_id_t id;
    enum mgc_dlgnode_type type;
    union {
        const char *text;
        const mgc_dlg_items_t *items;
    } params;
    bool terminal;
    uint32_t flags;
    mgc_node_id_t (*cb_before_switch_node)(const struct mgc_dlgflow *dlgflow);
} mgc_dlgnode_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DLGNODE_H*/
