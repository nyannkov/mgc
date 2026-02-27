#ifndef MGC_EQUIPMENT_MENU_HPP
#define MGC_EQUIPMENT_MENU_HPP

#include "game_context.hpp"
#include "resources/item_descriptors/item_descriptors_menu.h"

namespace app {

enum class MenuSelectMode {
    Item,
    Weapon
};

enum class SelectionState {
    Selecting,
    Confirmed,
    Cancelled,
};


struct EquipmentMenu {
    explicit EquipmentMenu(GameContext& ctx) 
        : sound_(ctx.platform.sound_controller),
          gamepad_(ctx.platform.gamepad),
          equip_info_(ctx.world_state.equipment_info) { }

    void init();
    void update();
    void draw(FramebufferT& fb);
    bool is_exit() const { return is_exit_; }

private:
    static constexpr size_t ITEM_COL_COUNT = 10;
    static constexpr size_t ITEM_ROW_COUNT = 2;
    static constexpr size_t WEAPON_COL_COUNT = 5;
    static constexpr size_t WEAPON_ROW_COUNT = 1;
    EquipmentInfo& equip_info_;
    const GamepadT& gamepad_;
    SoundControllerT& sound_;
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
    int active_cursor_index_ = 0;
    MenuSelectMode select_mode_ = MenuSelectMode::Item;
    SelectionState selection_state_ = SelectionState::Cancelled;

    void init_components();
    void toggle_mode();
    void set_selection_state(SelectionState state);
    void restore_cursor_position();
    void update_cursor_position();
    void confirm_selection();
    void update_description();

    void update_cursor(
        SpriteT& cursor,
        const mgc::math::Vec2i pos_orig,
        size_t selected_index
    );
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

