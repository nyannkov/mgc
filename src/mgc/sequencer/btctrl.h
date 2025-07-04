/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_BTCTRL_H
#define MGC_BTCTRL_H


#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "btree.h"

typedef struct mgc_btctrl mgc_btctrl_t;

enum mgc_btctrl_state {
    MGC_BTCTRL_STATE_INIT,
    MGC_BTCTRL_STATE_IN_PROGRESS,
    MGC_BTCTRL_STATE_FINISHED
};

typedef struct mgc_btctrl_callbacks {
    void* context;
    enum mgc_btree_leaf_state (*on_proc_leaf)(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context);
    void (*on_enter_leaf)(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context);
    void (*on_exit_leaf)(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context);
    void (*on_enter_node)(mgc_btctrl_t* btctrl, void *context);
    void (*on_exit_node)(mgc_btctrl_t* btctrl, void *context);
    void (*on_tree_start)(mgc_btctrl_t* btctrl, void *context);
    void (*on_tree_finish)(mgc_btctrl_t* btctrl, void *context);
} mgc_btctrl_callbacks_t;

struct mgc_btctrl {
    const mgc_btree_node_t* current;
    const mgc_btree_t* btree;
    enum mgc_btree_leaf_state last_leaf_state;
    enum mgc_btctrl_state state;
    const mgc_btctrl_callbacks_t *callbacks;
};

void btctrl_init(mgc_btctrl_t* btctrl);
void btctrl_set_btree(mgc_btctrl_t* btctrl, const mgc_btree_t* btree);
void btctrl_set_callbacks(mgc_btctrl_t* btctrl, const mgc_btctrl_callbacks_t* callbacks);
enum mgc_btctrl_state btctrl_proc(mgc_btctrl_t* btctrl);
void btctrl_reset_state(mgc_btctrl_t* btctrl);

static inline
enum mgc_btctrl_state
btctrl_get_state(const mgc_btctrl_t* btctrl) {
    MGC_ASSERT( btctrl != NULL , "Invalid handler");
    return btctrl->state;
}

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_BTCTRL_H*/
