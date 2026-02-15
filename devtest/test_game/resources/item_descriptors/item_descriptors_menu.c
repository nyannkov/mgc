#include "item_descriptors_menu.h"
#include "resources/generated/tileset/tileset_menu_items.h"

static const item_descriptor_t descriptors[COUNT_ITEM_DESCRIPTOR_MENU] = {
    {"test1", "テストアイテムその1", 1},
    {"test2", "テストアイテムその2", 2},
    {"test3", "テストアイテムその3", 3},
    {"test4", "テストアイテムその4", 4},
    {"test5", "テストアイテムその5", 5},
    {"test6", "テストアイテムその6", 6},
    {"test7", "テストアイテムその7", 7},
    {"test8", "テストアイテムその8", 8},
    {"test9", "テストアイテムその9", 9},
    {"test10", "テストアイテムその10", 10},
    {"test11", "テストアイテムその11", 11},
    {"test12", "テストアイテムその12", 12},
    {"test13", "テストアイテムその13", 13},
    {"test14", "テストアイテムその14", 14},
    {"test15", "テストアイテムその15", 15},
    {"test16", "テストアイテムその16", 16},
    {"test17", "テストアイテムその17", 17},
    {"test18", "テストアイテムその18", 18},
    {"test19", "テストアイテムその19", 19},
    {"test20", "テストアイテムその20", 20},
};

const item_descriptors_t item_descriptors_menu = {
    .tileset = &tileset_menu_items,
    .descriptors = descriptors,
    .descriptor_count = COUNT_ITEM_DESCRIPTOR_MENU
};


