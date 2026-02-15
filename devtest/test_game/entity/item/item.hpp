#ifndef MGC_ITEM_HPP
#define MGC_ITEM_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/player/player_hitbox_index.hpp"

namespace app {
namespace item {

enum class ItemHitboxId : size_t {
    Body = 0,
    Count
};

enum class ItemEffectType {
    Heal,
    LifeUp,
};

struct Item : mgc::entities::ActorImpl<
        Item, static_cast<size_t>(ItemHitboxId::Count)
    > {

    virtual ~Item() = default;

    virtual void spawn(const mgc::math::Vec2i& pos) = 0;
    virtual void despawn() = 0;
    virtual int32_t apply_effect_to(Player& player, size_t item_hitbox_index) const = 0;

    ItemEffectType effect_type() const { return effect_type_; }
    bool collected() const { return collected_; }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, Player>) {
            if ( info.other_hitbox_index == 
                static_cast<size_t>(PlayerHitboxIndex::Body) 
            ) {
                on_player_hit(other, info);
            }
        }
    }

protected:
    virtual void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    void set_effect_type(ItemEffectType effect_type) {
        effect_type_ = effect_type;
    }

    void set_collected(bool collected) {
        collected_ = collected;
    }

private:
    ItemEffectType effect_type_ = ItemEffectType::Heal;
    bool collected_ = false;
};

} // namespace item
} // namespace app

#endif // MGC_ITEM_HPP
