#ifndef MGC_EQUIPMENT_MENU_HPP
#define MGC_EQUIPMENT_MENU_HPP

#include "game_context/game_context.hpp"
#include "resources/item_descriptors/item_descriptors_menu.h"

namespace app {

enum class MenuSelectMode {
    Item,
    Weapon
};

enum class CursorState {
    Editing,
    Confirmed
};

enum class CursorResult {
    None,
    Confirm,
    Cancel
};

struct EquipmentMenu {
    explicit EquipmentMenu(
        SoundControllerT& sound,
        GamepadT& gamepad,
        Player& player
    ) : sound_(sound),
        gamepad_(gamepad),
        player_(player) { }

    void init(size_t item_selected_idx, size_t weapon_selected_idx);
    void update();
    void draw(FramebufferT& fb);
    bool is_exit() const { return is_exit_; }

    size_t item_selected_index() const { return item_selected_index_; }
    size_t weapon_selected_index() const { return weapon_selected_index_; }

private:
    static constexpr size_t ITEM_COL_COUNT = 10;
    static constexpr size_t ITEM_ROW_COUNT = 2;
    static constexpr size_t WEAPON_COL_COUNT = 5;
    static constexpr size_t WEAPON_ROW_COUNT = 1;
    SoundControllerT& sound_;
    GamepadT& gamepad_;
    Player& player_;
    LabelT title_items_;
    LabelT title_weapons_;
    LabelT item_name_;
    TilegridT name_plate_;
    SpriteT confirmed_cursor_item_;
    SpriteT confirmed_cursor_weapon_;
    SpriteT active_cursor_;
    SpriteT selected_mark_;
    SpriteT object_;
    DialogueboxT description_;
    bool is_exit_ = false;
    int item_selected_index_ = 0;
    int weapon_selected_index_ = 0;
    int active_cursor_index_ = 0;
    MenuSelectMode select_mode_ = MenuSelectMode::Item;
    CursorState cursor_state_ = CursorState::Confirmed;

    void init_components();
    CursorResult update_menu_state();
    void update_cursor(
        SpriteT& cursor,
        const mgc::math::Vec2i pos_orig,
        size_t selected_index
    );
    void update_description(const item_descriptors_t& desc, size_t selected_index);
    size_t select_object(
        const item_descriptors_t& desc,
        size_t row_count,
        size_t col_count,
        size_t current_index
    );
    void set_menu_select_mode(MenuSelectMode mode);
    void draw_object(
        FramebufferT& fb,
        const item_descriptors_t& desc,
        const mgc::math::Vec2i pos_orig,
        size_t row_count,
        size_t col_count
    );

};

}// namespace app

#endif/*MGC_EQUIPMENT_MENU_HPP*/

