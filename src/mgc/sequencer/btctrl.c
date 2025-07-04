/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "btctrl.h"


void btctrl_init(mgc_btctrl_t* btctrl) {
    if ( btctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    btctrl->current = NULL;
    btctrl->btree = NULL;
    btctrl->state = MGC_BTCTRL_STATE_INIT;
}

void btctrl_set_btree(mgc_btctrl_t* btctrl, const mgc_btree_t* btree) {
    if ( btctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    btctrl->btree = btree;
}

void btctrl_reset_state(mgc_btctrl_t* btctrl) {
    if ( btctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    btctrl->state = MGC_BTCTRL_STATE_INIT;
}

void btctrl_set_callbacks(mgc_btctrl_t* btctrl, const mgc_btctrl_callbacks_t* callbacks) {
    if ( btctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    btctrl->callbacks = callbacks;
}

static inline void go_to_first_child_leaf(mgc_btctrl_t* btctrl, const mgc_btctrl_callbacks_t* cbs) {

    while ( btctrl->current->type != MGC_BTREE_NODE_TYPE_LEAF ) {
        btctrl->current = btctrl->current->body.first_child;
        if ( cbs->on_enter_composite ) {
            cbs->on_enter_composite(btctrl, cbs->context);
        }
    }

    if ( cbs->on_enter_leaf ) {
        cbs->on_enter_leaf(btctrl, btctrl->current->body.leaf, cbs->context);
    }
}

static bool go_to_next_leaf(mgc_btctrl_t* btctrl, const mgc_btctrl_callbacks_t* cbs) {

    const mgc_btree_node_t* parent =
            btctrl->btree->parents[btctrl->current->parent_index];

    bool is_leaf_success =
        ( btctrl->last_leaf_state == MGC_BTREE_LEAF_RESULT_SUCCESS );

    if ( cbs->on_exit_leaf ) {
        cbs->on_exit_leaf(btctrl, btctrl->current->body.leaf, cbs->context);
    }

    while ( parent != NULL ) {

        if ( parent->type == MGC_BTREE_NODE_TYPE_SELECTOR ) {

            if ( is_leaf_success || ( btctrl->current->next_sibling == NULL ) ) {
                // selector succeeded or last child failed --> go up
                btctrl->current = parent;
                parent = btctrl->btree->parents[parent->parent_index];
                if ( cbs->on_exit_composite ) {
                    cbs->on_exit_composite(btctrl, cbs->context);
                }
            } else {
                // try next sibling
                btctrl->current = btctrl->current->next_sibling;
                go_to_first_child_leaf(btctrl, cbs);
                break;
            }
        } else if ( parent->type == MGC_BTREE_NODE_TYPE_SEQUENCER ) {

            if ( !is_leaf_success || ( btctrl->current->next_sibling == NULL ) ) {
                // sequencer failed or done --> go up
                btctrl->current = parent;
                parent = btctrl->btree->parents[parent->parent_index];
                if ( cbs->on_exit_composite ) {
                    cbs->on_exit_composite(btctrl, cbs->context);
                }
            } else {
                // next step
                btctrl->current = btctrl->current->next_sibling;
                go_to_first_child_leaf(btctrl, cbs);
                break;
            }
        } else {
            MGC_ASSERT(false, "Should never reach here");
        }
    }

    return ( parent != NULL );
}

enum mgc_btctrl_state btctrl_proc(mgc_btctrl_t* btctrl) {

    if ( ( btctrl == NULL ) ||
         ( btctrl->btree == NULL ) ||
         ( btctrl->btree->root == NULL ) ||
         ( btctrl->callbacks == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return MGC_BTCTRL_STATE_INIT;
    }

    const mgc_btctrl_callbacks_t* cbs = btctrl->callbacks;

    if ( btctrl->state == MGC_BTCTRL_STATE_FINISHED ) {

        return btctrl->state;

    } else if ( btctrl->state == MGC_BTCTRL_STATE_INIT ) {

        btctrl->current = btctrl->btree->root;
        btctrl->last_leaf_state = MGC_BTREE_LEAF_RESULT_INIT;
        btctrl->state = MGC_BTCTRL_STATE_IN_PROGRESS;
        if ( cbs->on_tree_start ) {
            cbs->on_tree_start(btctrl, cbs->context);
        }

        go_to_first_child_leaf(btctrl, cbs);

        btctrl->last_leaf_state = MGC_BTREE_LEAF_RESULT_RUNNING;

        if ( cbs->on_proc_leaf ) {
            btctrl->last_leaf_state =
            cbs->on_proc_leaf(btctrl, btctrl->current->body.leaf, cbs->context);
        }

    } else if ( btctrl->state == MGC_BTCTRL_STATE_IN_PROGRESS ) {

        if ( btctrl->last_leaf_state == MGC_BTREE_LEAF_RESULT_RUNNING ) {
            if ( cbs->on_proc_leaf ) {
                btctrl->last_leaf_state =
                cbs->on_proc_leaf(btctrl, btctrl->current->body.leaf, cbs->context);
            }
        } else {

            if ( go_to_next_leaf(btctrl, cbs) ) {

                if ( cbs->on_proc_leaf ) {
                    btctrl->last_leaf_state =
                    cbs->on_proc_leaf(btctrl, btctrl->current->body.leaf, cbs->context);
                }

            } else {
                btctrl->state = MGC_BTCTRL_STATE_FINISHED;
                if ( cbs->on_tree_finish ) {
                    cbs->on_tree_finish(btctrl, cbs->context);
                }
            }
        }

    } else {
        MGC_ASSERT(false, "Should never reach here");
    }

    return btctrl->state;
}

