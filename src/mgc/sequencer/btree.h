/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_BTREE_H
#define MGC_BTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef int32_t mgc_btree_tag_t;
typedef struct mgc_btree_node mgc_btree_node_t;

enum mgc_btree_node_type {
    MGC_BTREE_NODE_TYPE_LEAF,
    MGC_BTREE_NODE_TYPE_SELECTOR,
    MGC_BTREE_NODE_TYPE_SEQUENCER
};

enum mgc_btree_leaf_result {
    MGC_BTREE_LEAF_RESULT_INIT,
    MGC_BTREE_LEAF_RESULT_RUNNING,
    MGC_BTREE_LEAF_RESULT_SUCCESS,
    MGC_BTREE_LEAF_RESULT_FAILURE,
};

typedef struct mgc_btree_leaf {
    const char *id;
} mgc_btree_leaf_t;

struct mgc_btree_node {
    mgc_btree_tag_t tag;
    size_t parent_index;
    const mgc_btree_node_t* next_sibling;
    enum mgc_btree_node_type type;
    union {
        const mgc_btree_leaf_t* leaf;
        const mgc_btree_node_t* first_child;
    } body;
};

typedef struct mgc_btree {
    const mgc_btree_node_t* root;
    const mgc_btree_node_t** parents;
    size_t parents_count;
} mgc_btree_t;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MGC_BTREE_H */

