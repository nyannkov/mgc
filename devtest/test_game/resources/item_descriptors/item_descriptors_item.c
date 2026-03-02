#include "item_descriptors_item.h"
#include "resources/generated/tileset/tileset_menu_items.h"

static const item_descriptor_t descriptors_items[COUNT_ITEM_DESCRIPTOR_ITEM] = {
    {"test1", "テストアイテムその1", 0},
    {"test2", "テストアイテムその2", 1},
    {"test3", "テストアイテムその3", 2},
    {"test4", "テストアイテムその4", 3},
    {"test5", "テストアイテムその5", 4},
    {"test6", "テストアイテムその6", 5},
    {"test7", "テストアイテムその7", 6},
    {"test8", "テストアイテムその8", 7},
    {"test9", "テストアイテムその9", 8},
    {"test10", "テストアイテムその10", 9},
    {"test11", "テストアイテムその11", 10},
    {"test12", "テストアイテムその12", 11},
    {"test13", "テストアイテムその13", 12},
    {"test14", "テストアイテムその14", 13},
    {"test15", "テストアイテムその15", 14},
    {"test16", "テストアイテムその16", 15},
    {"test17", "テストアイテムその17", 16},
    {"test18", "テストアイテムその18", 17},
    {"test19", "テストアイテムその19", 18},
    {"test20", "テストアイテムその20", 19},
};

const item_descriptors_t item_descriptors_item = {
    .title = "ITEM:",
    .tileset = &tileset_menu_items,
    .descriptors = descriptors_items,
    .descriptor_count = COUNT_ITEM_DESCRIPTOR_ITEM
};

