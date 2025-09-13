#ifndef MGC_ITEM_POTION_HPP
#define MGC_ITEM_POTION_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/item/item.hpp"

namespace app {
namespace item {

struct Potion : Item {

    Potion();

    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    int32_t apply_effect_to(Player& player, size_t item_hitbox_index) const override;

protected:
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

private:
    ItemEffectType effect_type_ = ItemEffectType::Heal;
};

} // namespace item
} // namespace app

#endif // MGC_ITEM_POTION_HPP
