#ifndef MGC_EQUIPMENT_INFO_HPP
#define MGC_EQUIPMENT_INFO_HPP

namespace app {

struct EquipmentInfo {
    size_t item_selected_index() const { return item_selected_index_; }
    size_t weapon_selected_index() const { return weapon_selected_index_; }
    void set_item_selected_index(size_t index) { item_selected_index_ = index; }
    void set_weapon_selected_index(size_t index) { weapon_selected_index_ = index; }
    
private:
    size_t item_selected_index_ = 0;
    size_t weapon_selected_index_ = 0;
};

}// namespace app

#endif/*MGC_EQUIPMENT_INFO_HPP*/

