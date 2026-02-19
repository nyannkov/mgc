#include "equipment_menu.hpp"
#include "resources/generated/font/k8x12.h"
#include "resources/generated/tileset/tileset_menu_display.h"
#include "resources/generated/tileset/tileset_menu_cursor.h"
#include "resources/generated/map/item_name_plate.h"

namespace app {

using mgc::platform::input::Key;

void EquipmentMenu::init_components() {
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

    description_.set_position({8, 136});
    description_.set_size(208, 48);
    description_.set_font(k8x12);
    description_.set_fontsize2x(false);
    description_.set_typing_speed(0);

    confirmed_cursor_item_.set_tileset(tileset_menu_cursor);
    confirmed_cursor_item_.set_tile_index(0);

    confirmed_cursor_weapon_.set_tileset(tileset_menu_cursor);
    confirmed_cursor_weapon_.set_tile_index(0);

    active_cursor_.set_tileset(tileset_menu_cursor);
    active_cursor_.set_tile_index(1);
}

void EquipmentMenu::init(size_t item_selected_idx, size_t weapon_selected_idx) {

    init_components();

    is_exit_ = false;

    item_selected_index_ = item_selected_idx;
    active_cursor_index_ = item_selected_idx; // Item mode
    weapon_selected_index_ = weapon_selected_idx;

    cursor_state_ = CursorState::Confirmed;
    active_cursor_.set_visible(false);

    set_menu_select_mode(MenuSelectMode::Item);
    update_description(item_descriptors_menu, item_selected_index_);

    update_cursor(active_cursor_, {5, 48}, item_selected_index_);
    update_cursor(confirmed_cursor_item_, {5, 48}, item_selected_index_);
    update_cursor(confirmed_cursor_weapon_, {5, 112}, weapon_selected_index_);
}

void EquipmentMenu::set_menu_select_mode(MenuSelectMode mode) {
    select_mode_ = mode;
    if ( select_mode_ == MenuSelectMode::Item ) {
        title_items_.set_font_fore_color(MGC_COLOR_WHITE);
        title_weapons_.set_font_fore_color(MGC_COLOR(0.5, 0.5, 0.5));
    } else {
        title_items_.set_font_fore_color(MGC_COLOR(0.5, 0.5, 0.5));
        title_weapons_.set_font_fore_color(MGC_COLOR_WHITE);
    }
}

void EquipmentMenu::update_description(const item_descriptors_t& desc, size_t selected_index) {
    description_.set_text(desc.descriptors[selected_index].description);
    item_name_.set_text(desc.descriptors[selected_index].name);
    item_name_.adjust_size_to_fit();
    auto pos = item_name_.position();
    pos.x = 112 - item_name_.size().width/2;
    item_name_.set_position(pos);
}

void EquipmentMenu::update_cursor(
    SpriteT& cursor,
    const mgc::math::Vec2i pos_orig,
    size_t selected_index
) {
    mgc::math::Vec2i cursor_pos;
    cursor_pos.x = (selected_index%10)*16+pos_orig.x;
    cursor_pos.y = (selected_index/10)*16+pos_orig.y;
    cursor.set_position(cursor_pos);
}

CursorResult EquipmentMenu::update_menu_state() {
    
    bool mode_changed = false;
    auto cursor_result = CursorResult::None;

    if ( gamepad_.just_pressed(Key::Menu) ) {
        mode_changed = true;
    }

    if ( mode_changed ) {
        if ( select_mode_ == MenuSelectMode::Item ) {
            set_menu_select_mode(MenuSelectMode::Weapon);
            active_cursor_index_ = item_selected_index_;
        } else {
            set_menu_select_mode(MenuSelectMode::Item);
            active_cursor_index_ = weapon_selected_index_;
        }

        if ( cursor_state_ == CursorState::Editing ) {
            cursor_state_ = CursorState::Confirmed;
            cursor_result = CursorResult::Cancel;
        }
    } else {
        if ( cursor_state_ == CursorState::Confirmed ) {
            if (  gamepad_.just_pressed(Key::Right) || 
                  gamepad_.just_pressed(Key::Left) || 
                  gamepad_.just_pressed(Key::Up) || 
                  gamepad_.just_pressed(Key::Down) 
            ) {
                cursor_state_ = CursorState::Editing;
            } else if ( gamepad_.just_pressed(Key::Cancel) ) {
                is_exit_ = true;
            }
        } else {
            if ( gamepad_.just_pressed(Key::Enter) ) {
                cursor_state_ = CursorState::Confirmed;
                cursor_result = CursorResult::Confirm;

            } else if ( gamepad_.just_pressed(Key::Cancel) ) {
                cursor_state_ = CursorState::Confirmed;
                cursor_result = CursorResult::Cancel;
            } else { }
        }
    }

    return cursor_result;
}

void EquipmentMenu::update() {
    
    auto cursor_result = update_menu_state();
    if ( cursor_state_ == CursorState::Editing ) {
        active_cursor_.set_visible(true);
        if ( select_mode_ == MenuSelectMode::Item ) {
            active_cursor_index_ = select_object(
                item_descriptors_menu,
                ITEM_ROW_COUNT,
                ITEM_COL_COUNT,
                active_cursor_index_
            );
            update_description(item_descriptors_menu, active_cursor_index_);
            update_cursor(active_cursor_, {5, 48}, active_cursor_index_);
        } else {
            active_cursor_index_ = select_object(
                weapon_descriptors_menu,
                WEAPON_ROW_COUNT,
                WEAPON_COL_COUNT,
                active_cursor_index_
            );
            update_description(weapon_descriptors_menu, active_cursor_index_);
            update_cursor(active_cursor_, {5, 112}, active_cursor_index_);
        }
    } else {
        active_cursor_.set_visible(false);
        if ( select_mode_ == MenuSelectMode::Item ) {
            if ( cursor_result == CursorResult::Confirm ) {
                item_selected_index_ = active_cursor_index_;
            } else if ( cursor_result == CursorResult::Cancel ) {
                active_cursor_index_ = item_selected_index_;
            }
            item_selected_index_ = select_object(
                item_descriptors_menu,
                ITEM_ROW_COUNT,
                ITEM_COL_COUNT,
                item_selected_index_
            );
            update_description(item_descriptors_menu, item_selected_index_);
            update_cursor(confirmed_cursor_item_, {5, 48}, item_selected_index_);
        } else {
            if ( cursor_result == CursorResult::Confirm ) {
                weapon_selected_index_ = active_cursor_index_;
            } else if ( cursor_result == CursorResult::Cancel ) {
                active_cursor_index_ = weapon_selected_index_;
            }
            weapon_selected_index_ = select_object(
                weapon_descriptors_menu,
                ITEM_ROW_COUNT,
                ITEM_COL_COUNT,
                weapon_selected_index_
            );
            update_description(weapon_descriptors_menu, weapon_selected_index_);
            update_cursor(confirmed_cursor_weapon_, {5, 112}, weapon_selected_index_);
        }
    }

    description_.advance_typing();
}

size_t EquipmentMenu::select_object(
    const item_descriptors_t& desc,
    size_t row_count,
    size_t col_count,
    size_t current_index
) {
    size_t selected_index = current_index;
    size_t row_num = selected_index / col_count;
    size_t col_num = selected_index % col_count;

    if ( gamepad_.just_pressed(Key::Right) ) {
        selected_index++;
        if ( selected_index >= desc.descriptor_count ) {
            selected_index = 0;
        }
    } else if ( gamepad_.just_pressed(Key::Left) ) {
        if ( selected_index == 0 ) {
            selected_index = desc.descriptor_count - 1;
        } else {
            selected_index--;
        }
    } else if ( gamepad_.just_pressed(Key::Up) ) {
        if ( row_num == 0 ) {
            row_num = row_count - 1;
        } else {
            row_num--;
        }
        selected_index = row_num * col_count + col_num;
    } else if ( gamepad_.just_pressed(Key::Down) ) {
        if ( row_num == row_count-1 ) {
            row_num = 0;
        } else {
            row_num++;
        }
        selected_index = row_num * col_count + col_num;
    }

    return selected_index;
}

void EquipmentMenu::draw_object(
    FramebufferT& fb,
    const item_descriptors_t& desc,
    const mgc::math::Vec2i pos_orig,
    size_t row_count,
    size_t col_count
) {
    size_t i = 0;

    object_.set_tileset(*desc.tileset);

    for ( int16_t y = 0; y < row_count; y++ ) {
        for ( int16_t x = 0; x < col_count; x++ ) {
            mgc::math::Vec2i obj_pos;
            obj_pos.x = 16 * x + pos_orig.x;
            obj_pos.y = 16 * y + pos_orig.y;
            object_.set_tile_index(desc.descriptors[i++].tile_idx);
            object_.set_position(obj_pos);
            object_.draw(fb);
            if ( i >= desc.descriptor_count ) {
                break;
            }
        }
    }
}

void EquipmentMenu::draw(FramebufferT& fb) {
    
    fb.clear(MGC_COLOR_BLACK);

    draw_object(
        fb, 
        item_descriptors_menu,
        {5, 48},
        ITEM_ROW_COUNT,
        ITEM_COL_COUNT
    );
    draw_object(
        fb, 
        weapon_descriptors_menu,
        {5, 112},
        WEAPON_ROW_COUNT,
        WEAPON_COL_COUNT
    );

    description_.draw(fb);
    name_plate_.draw(fb);
    item_name_.draw(fb);
    title_items_.draw(fb);
    title_weapons_.draw(fb);

    confirmed_cursor_item_.draw(fb);
    confirmed_cursor_weapon_.draw(fb);
    active_cursor_.draw(fb);
}

}// namespace app

