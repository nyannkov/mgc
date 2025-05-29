/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_TALKSCRIPT_H
#define MGC_TALKSCRIPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef size_t mgc_node_idx_t;

enum mgc_talknode_type {
    MGC_TALKNODE_TYPE_MESSAGE,
    MGC_TALKNODE_TYPE_CHOICE,
    MGC_TALKNODE_TYPE_DECISION,
};

typedef struct mgc_node_choice_item {
    const char *text;
    int32_t value;
    mgc_node_idx_t next;
} mgc_node_choice_item_t;

typedef struct mgc_node_choice {
    const mgc_node_choice_item_t *items;
    size_t item_count;
} mgc_node_choice_t;

typedef struct mgc_node_message {
    const char *text;
    bool format;
    mgc_node_idx_t next;
} mgc_node_message_t;

typedef struct mgc_node_decision {
    mgc_node_idx_t next_if_true;
    mgc_node_idx_t next_if_false;
} mgc_node_decision_t;

typedef struct mgc_talknode {
    union {
        const mgc_node_message_t *message;
        const mgc_node_choice_t *choice;
        const mgc_node_decision_t *decision;
    } content;
    enum mgc_talknode_type type;
    bool end;
} mgc_talknode_t;

typedef struct mgc_talkscript {
    const mgc_talknode_t *nodes;
    size_t node_count;
} mgc_talkscript_t;

static inline 
int32_t talkscript_get_item_value(const mgc_node_choice_t *choice, size_t item_tag) {
    if ( ( choice == NULL ) ||
         ( choice->items == NULL ) ||
         ( choice->item_count <= item_tag )
    ) {
        MGC_WARN("Invalid handler");
        return 0;
    }

    return choice->items[item_tag].value;
}


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_TALKSCRIPT_H*/
