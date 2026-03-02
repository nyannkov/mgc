#ifndef MGC_EQUIPMENT_MENU_HPP
#define MGC_EQUIPMENT_MENU_HPP

#include "game_context.hpp"
#include "item_slot.hpp"

namespace app {

enum class MenuSelectMode {
    Item,
    Weapon
};

struct EquipmentMenu {
    explicit EquipmentMenu(GameContext& ctx);
    void init();
    void update();
    void draw(FramebufferT& fb);
    bool is_exit() const { return is_exit_; }

private:
    const GamepadT& gamepad_;
    SoundControllerT& sound_;
    TilegridT name_plate_;
    LabelT item_name_;
    DialogueboxT description_;
    ItemSlot<EquipSlot<ItemId>, 2, 10> item_slot_;
    ItemSlot<EquipSlot<WeaponId>, 1, 5> weapon_slot_;
    bool is_exit_ = false;
    MenuSelectMode select_mode_ = MenuSelectMode::Item;

    void toggle_mode();
    void update_description();
    void set_menu_select_mode(MenuSelectMode mode);
};

}// namespace app

#endif/*MGC_EQUIPMENT_MENU_HPP*/

