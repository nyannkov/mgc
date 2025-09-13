#include "potion.hpp"
#include "entity/player/player.hpp"
#include "resources/generated/tileset/tileset_items.h"

namespace app {
namespace item {

Potion::Potion() {

    this->set_position({0, 0});
    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_items);
    this->sprite().set_tile_index(3);

    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(8, 8);
    hitboxes[0].enabled = false;
}

void Potion::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = true;
    this->set_position(pos);
    this->sprite().set_visible(true);
}

void Potion::despawn() {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = false;
    this->sprite().set_visible(false);
}

int32_t Potion::apply_effect_to(Player& player, size_t item_hitbox_index) const {
    int32_t amount = 0;
    if ( item_hitbox_index == 0 ) {
        amount = 1;
        player.receive_heal(amount);
    }
    return amount;
}

void Potion::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    this->despawn();
}

} // namespace item
} // namespace app

