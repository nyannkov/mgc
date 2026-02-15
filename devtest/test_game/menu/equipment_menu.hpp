#ifndef MGC_EQUIPMENT_MENU_HPP
#define MGC_EQUIPMENT_MENU_HPP

#include "game_context/game_context.hpp"

namespace app {

struct EquipmentMenu {
    explicit EquipmentMenu(
        SoundControllerT& sound,
        GamepadT& gamepad,
        Player& player
    ) : sound_(sound),
        gamepad_(gamepad),
        player_(player) { }

    void init();
    void update();
    void draw(FramebufferT& fb);
    bool is_exit() const { return is_exit_; }

private:
    static constexpr size_t ITEM_COL_NUM = 10;
    SoundControllerT& sound_;
    GamepadT& gamepad_;
    Player& player_;
    LabelT title_items_;
    LabelT title_weapons_;
    LabelT item_name_;
    TilegridT name_plate_;
    SpriteT cursor_;
    SpriteT selected_mark_;
    SpriteT object_;
    SpriteT object_weapon_;//TODO
    DialogueboxT description_;
    bool is_exit_ = false;
    int item_selected_index_ = 0;

    void update_selected_item(size_t selected_index);
};

}// namespace app

#endif/*MGC_EQUIPMENT_MENU_HPP*/

