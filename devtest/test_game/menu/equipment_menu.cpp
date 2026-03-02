#include "equipment_menu.hpp"
#include "resources/generated/font/k8x12.h"
#include "resources/generated/tileset/tileset_menu_display.h"
#include "resources/generated/tileset/tileset_menu_cursor.h"
#include "resources/generated/map/item_name_plate.h"
#include "resources/item_descriptors/item_descriptors_item.h"
#include "resources/item_descriptors/item_descriptors_weapon.h"

namespace app {

using mgc::platform::input::Key;

EquipmentMenu::EquipmentMenu(GameContext& ctx)
    : sound_(ctx.platform.sound_controller),
      gamepad_(ctx.platform.gamepad),
      item_slot_(
        ctx.world_state.equipment_info.item, 
        item_descriptors_item
      ),
      weapon_slot_(
        ctx.world_state.equipment_info.weapon,
        item_descriptors_weapon
      ) { }

void EquipmentMenu::init() {

    name_plate_.set_position({16*3+8, 8});
    name_plate_.set_tileset(tileset_menu_display);
    name_plate_.set_tile_index_map(item_name_plate);

    item_name_.set_position({16*5, 10});
    item_name_.set_size(100, 16);
    item_name_.set_font(k8x12);
    item_name_.set_font_fore_color(MGC_COLOR_WHITE);
    item_name_.set_fontsize2x(false);

    description_.set_position({8, 136});
    description_.set_size(208, 48);
    description_.set_font(k8x12);
    description_.set_fontsize2x(false);
    description_.set_typing_speed(0);

    // The default mode is item select mode.
    item_slot_.init(true, {0, 32});
    weapon_slot_.init(false, {0, 96});
    set_menu_select_mode(MenuSelectMode::Item);

    update_description();

    is_exit_ = false;
}

void EquipmentMenu::update() {
    
    SelectionState selection_state = SelectionState::Cancelled;

    if ( gamepad_.just_pressed(Key::Menu) ) {
        toggle_mode();
    }

    if ( select_mode_ == MenuSelectMode::Item ) {
        item_slot_.update(gamepad_);
        selection_state = item_slot_.selection_state();
    } else {
        weapon_slot_.update(gamepad_);
        selection_state = weapon_slot_.selection_state();
    }

    if ( selection_state == SelectionState::Exit ) {
        is_exit_ = true;
    }

    update_description();

    description_.advance_typing();
}

void EquipmentMenu::draw(FramebufferT& fb) {
    
    fb.clear(MGC_COLOR_BLACK);

    item_slot_.draw(fb);
    weapon_slot_.draw(fb);
    description_.draw(fb);
    name_plate_.draw(fb);
    item_name_.draw(fb);
}

void EquipmentMenu::set_menu_select_mode(MenuSelectMode mode) {
    select_mode_ = mode;
    if ( select_mode_ == MenuSelectMode::Item ) {
        item_slot_.focus();
        weapon_slot_.unfocus();
    } else {
        item_slot_.unfocus();
        weapon_slot_.focus();
    }
}

void EquipmentMenu::update_description() {

    const item_descriptor_t* desc = nullptr;

    if ( select_mode_ == MenuSelectMode::Item ) {
        desc = item_slot_.current_desc();
    } else {
        desc = weapon_slot_.current_desc();
    }

    if ( desc == nullptr ) {
        description_.set_text("");
        item_name_.set_text("");
        item_name_.adjust_size_to_fit();
    } else {
        description_.set_text(desc->description);
        item_name_.set_text(desc->name);
        item_name_.adjust_size_to_fit();
        auto pos = item_name_.position();
        pos.x = 112 - item_name_.size().width/2;
        item_name_.set_position(pos);
    }
}

void EquipmentMenu::toggle_mode() {
    if ( select_mode_ == MenuSelectMode::Item ) {
        set_menu_select_mode(MenuSelectMode::Weapon);
    } else {
        set_menu_select_mode(MenuSelectMode::Item);
    }
}

}// namespace app

