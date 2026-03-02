#ifndef MGC_EQUIPMENT_INFO_HPP
#define MGC_EQUIPMENT_INFO_HPP

namespace app {

enum class ItemId : uint32_t {
    Test1 = 0,
    Test2,
    Test3,
    Count,
    None
};

enum class WeaponId : uint32_t {
    Claw = 0,
    Boomerang,
    Count,
    None
};

template <typename IdT>
struct EquipSlot {
    bool equip(IdT id) {
        return equip(static_cast<uint32_t>(id));
    }

    bool equip(uint32_t id) {
        if ( !is_valid(id) ) {
            return false;
        }
        equipped_id_ = id;
        equipped_ = true;
        return true;
    }

    void unequip() {
        equipped_id_ = IdT::None;
        equipped_ = false;
    }

    bool equipped() const { return equipped_; }
    uint32_t equipped_id() const { return equipped_id_; }

    bool add(IdT id) {
        if ( !is_valid(id) ) {
            return false;
        }
        owned_flags_ |= 1 << static_cast<uint32_t>(id);
        return true;
    }

    bool remove(IdT id) {
        if ( !is_valid(id) ) {
            return false;
        }
        owned_flags_ &= ~(1 << static_cast<uint32_t>(id));
        return true;
    }

    bool has_item_at(IdT id) const {
        if ( !is_valid(id) ) {
            return false;
        }
        return has_item_at(static_cast<uint32_t>(id));
    }

    bool has_item_at(uint32_t id) const {
        if ( !is_valid(id) ) {
            return false;
        }
        return owned_flags_ & (1 << id);
    }

    bool is_empty() const { return owned_flags_ == 0; }

private:
    uint32_t equipped_id_ = 0;
    uint32_t owned_flags_ = 0;
    bool equipped_ = false;
    bool is_valid(IdT id) const {
        return ( static_cast<uint32_t>(id) < static_cast<uint32_t>(IdT::Count) );
    }
    bool is_valid(uint32_t id) const {
        return ( id < static_cast<uint32_t>(IdT::Count) );
    }
};

struct EquipmentInfo {
    EquipSlot<ItemId> item;
    EquipSlot<WeaponId> weapon;
};

}// namespace app

#endif/*MGC_EQUIPMENT_INFO_HPP*/

