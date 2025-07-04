#include <stdio.h>
#include "mgc/mgc.h"
#include "./resources/generated/btree/test_btree.h"

static int counter = 0;

static enum mgc_btree_leaf_state on_proc_leaf(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context) {
    printf("%s, counter=%d\n", leaf->id, counter);

    if ( counter++ < 10 ) {
        return MGC_BTREE_LEAF_RESULT_FAILURE;
    } else {
        return MGC_BTREE_LEAF_RESULT_SUCCESS;
    }
}
static void on_enter_leaf(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context) {
    counter = 0;
    printf("on_enter_leaf(%d)\n", btctrl->current->tag);
}
static void on_exit_leaf(mgc_btctrl_t* btctrl, const mgc_btree_leaf_t* leaf, void *context) {
    printf("on_exit_leaf(%d)\n", btctrl->current->tag);
}
static void on_enter_node(mgc_btctrl_t* btctrl, void *context) {
    printf("on_enter_node(%d)\n", btctrl->current->tag);
}
static void on_exit_node(mgc_btctrl_t* btctrl, void *context) {
    printf("on_exit_node(%d)\n", btctrl->current->tag);
}
static void on_tree_start(mgc_btctrl_t* btctrl, void *context) {
    printf("on_tree_start(%d)\n", btctrl->current->tag);
}
static void on_tree_finish(mgc_btctrl_t* btctrl, void *context) {
    printf("on_tree_finish(%d)\n", btctrl->current->tag);
}

static const mgc_btctrl_callbacks_t callbacks  = {
    NULL,
    on_proc_leaf,
    on_enter_leaf,
    on_exit_leaf,
    on_enter_node,
    on_exit_node,
    on_tree_start,
    on_tree_finish
};

int main(void) {

    mgc_btctrl_t btctrl;

    btctrl_init(&btctrl);

    btctrl_set_btree(&btctrl, &test_btree);

    btctrl_set_callbacks(&btctrl, &callbacks);

    while (btctrl_proc(&btctrl) != MGC_BTCTRL_STATE_FINISHED) { }

    return 0;
}
