#include "equipment_menu.hpp"
#include "resources/item_descriptors/item_descriptors_menu.h"
#include "resources/generated/font/k8x12.h"
#include "resources/generated/tileset/tileset_menu_display.h"
#include "resources/generated/tileset/tileset_menu_weapon.h"//TODO remove
#include "resources/generated/map/item_name_plate.h"

namespace app {

using mgc::platform::input::Key;

void EquipmentMenu::init() {

    name_plate_.set_position({16*3+8, 8});
    name_plate_.set_tileset(tileset_menu_display);
    name_plate_.set_tile_index_map(item_name_plate);

    item_name_.set_position({16*5, 10});
    item_name_.set_size(100, 16);
    item_name_.set_font(k8x12);
    item_name_.set_font_fore_color(MGC_COLOR_WHITE);
    item_name_.set_fontsize2x(false);

    title_items_.set_text("ITEMS:");
    title_items_.adjust_size_to_fit();
    title_items_.set_position({0, 32});
    title_items_.set_font(k8x12);
    title_items_.set_font_fore_color(MGC_COLOR_WHITE);
    title_items_.set_fontsize2x(false);

    title_weapons_.set_text("WEAPONS:");
    title_weapons_.adjust_size_to_fit();
    title_weapons_.set_position({0, 96});
    title_weapons_.set_font(k8x12);
    title_weapons_.set_font_fore_color(MGC_COLOR_WHITE);
    title_weapons_.set_fontsize2x(false);
    //TODO
    object_weapon_.set_tileset(tileset_menu_weapon);
    object_weapon_.set_position({5, 112});

    
    description_.set_position({8, 136});
    description_.set_size(208, 48);
    description_.set_font(k8x12);
    description_.set_fontsize2x(false);
    description_.set_typing_speed(0);

    object_.set_tileset(*item_descriptors_menu.tileset);
    cursor_.set_tileset(*item_descriptors_menu.tileset);
    cursor_.set_tile_index(0);

    is_exit_ = false;

    item_selected_index_ = 0;

    update_selected_item(item_selected_index_);
}

void EquipmentMenu::update_selected_item(size_t selected_index) {
    description_.set_text(item_descriptors_menu.descriptors[item_selected_index_].description);
    item_name_.set_text(item_descriptors_menu.descriptors[item_selected_index_].name);
    item_name_.adjust_size_to_fit();
    auto pos = item_name_.position();
    pos.x = 112 - item_name_.size().width/2;
    item_name_.set_position(pos);

    mgc::math::Vec2i cursor_pos;
    cursor_pos.x = (item_selected_index_%10)*16+5;
    cursor_pos.y = (item_selected_index_/10)*16+48;
    cursor_.set_position(cursor_pos);
}

void EquipmentMenu::update() {
    
    size_t selected_index = item_selected_index_;


    if ( gamepad_.just_pressed(Key::Right) ) {
        selected_index++;
        if ( selected_index >= item_descriptors_menu.descriptor_count ) {
            selected_index = 0;
        }
    } else if ( gamepad_.just_pressed(Key::Left) ) {
        if ( selected_index == 0 ) {
            selected_index = item_descriptors_menu.descriptor_count - 1;
        } else {
            selected_index--;
        }
    } else if ( gamepad_.just_pressed(Key::Up) || 
                gamepad_.just_pressed(Key::Down) 
    ) {
        if ( selected_index < ITEM_COL_NUM ) {
            selected_index += ITEM_COL_NUM;
        } else {
            selected_index -= ITEM_COL_NUM;
        }
    } else if ( gamepad_.just_pressed(Key::Down) ) {
        if ( selected_index < ITEM_COL_NUM ) {
            selected_index += ITEM_COL_NUM;
        } else {
            selected_index -= ITEM_COL_NUM;
        }
    }

    if ( item_selected_index_ != selected_index ) {
        item_selected_index_ = selected_index;
        update_selected_item(item_selected_index_);
    }

    description_.advance_typing();
    if ( gamepad_.just_pressed(Key::Menu) ) {
        is_exit_ = true;
    }
}

void EquipmentMenu::draw(FramebufferT& fb) {
    
    size_t i = 0;
    fb.clear(MGC_COLOR_BLACK);

    for ( int16_t y = 0; y < 2; y++ ) {
        for ( int16_t x = 0; x < 10; x++ ) {
            mgc::math::Vec2i obj_pos;
            obj_pos.x = 16 * x + 5;
            obj_pos.y = 16 * y + 48;
            object_.set_tile_index(item_descriptors_menu.descriptors[i++].tile_idx);
            object_.set_position(obj_pos);
            object_.draw(fb);
            if ( i >= item_descriptors_menu.descriptor_count ) {
                break;
            }
        }
    }

    cursor_.draw(fb);
    description_.draw(fb);
    name_plate_.draw(fb);
    item_name_.draw(fb);
    title_items_.draw(fb);
    title_weapons_.draw(fb);
    object_weapon_.draw(fb);//TODO
}

}// namespace app

