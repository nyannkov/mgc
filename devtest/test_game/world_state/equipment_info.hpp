#ifndef MGC_EQUIPMENT_INFO_HPP
#define MGC_EQUIPMENT_INFO_HPP

namespace app {

enum class ItemBit : uint32_t {
    Test1 = 0,
    Test2 = 1,
    Test3 = 2,
};

enum class WeaponBit : uint32_t {
    Claw = 0,
    Boomerang = 1,
};

struct EquipmentInfo {
    size_t item_selected_index() const { return item_selected_index_; }
    size_t weapon_selected_index() const { return weapon_selected_index_; }
    void set_item_selected_index(size_t index) { item_selected_index_ = index; }
    void set_weapon_selected_index(size_t index) { weapon_selected_index_ = index; }

    uint32_t owned_weapon_flags() const { return owned_weapon_flags_; }
    uint32_t owned_item_flags() const { return owned_item_flags_; }
    void set_owned_weapon_flags(uint32_t owned_flags) {
        owned_weapon_flags_ = owned_flags;
    }
    void set_owned_item_flags(uint32_t owned_flags) {
        owned_item_flags_ = owned_flags;
    }
    void set_owned_item_bit(ItemBit bit) {
        owned_item_flags_ |= 1 << static_cast<uint32_t>(bit);
    }
    void clear_owned_item_bit(ItemBit bit) {
        owned_item_flags_ &= ~(1 << static_cast<uint32_t>(bit));
    }
    void set_owned_weapon_bit(WeaponBit bit) {
        owned_weapon_flags_ |= 1 << static_cast<uint32_t>(bit);
    }
    void clear_owned_weapon_bit(WeaponBit bit) {
        owned_weapon_flags_ &= ~(1 << static_cast<uint32_t>(bit));
    }
private:
    size_t item_selected_index_ = 0;
    size_t weapon_selected_index_ = 0;
    uint32_t owned_weapon_flags_ = 0x1;// Claw
    uint32_t owned_item_flags_ = 0;
};

}// namespace app

#endif/*MGC_EQUIPMENT_INFO_HPP*/

