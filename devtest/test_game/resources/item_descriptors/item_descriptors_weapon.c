#include "item_descriptors_weapon.h"
#include "resources/generated/tileset/tileset_menu_weapon.h"

static const item_descriptor_t descriptors_weapons[COUNT_ITEM_DESCRIPTOR_WEAPON] = {
    { "Claw",        "テスト1", 0 },
    { "Boomerang",   "テスト2", 1 },
};

const item_descriptors_t item_descriptors_weapon = {
    .title = "WEAPON:",
    .tileset = &tileset_menu_weapon,
    .descriptors = descriptors_weapons,
    .descriptor_count = COUNT_ITEM_DESCRIPTOR_WEAPON
};

