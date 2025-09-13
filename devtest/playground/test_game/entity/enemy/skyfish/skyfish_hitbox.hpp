#ifndef MGC_ENEMY_SKY_FISH_HITBOX_INDEX_HPP
#define MGC_ENEMY_SKY_FISH_HITBOX_INDEX_HPP

#include "entity/enemy/enemy.hpp"

namespace app {
namespace enemy {

enum class SkyFishHitboxIndex : size_t {
    Body = 0,
    ViewLeft,
    ViewRight,
    Count
};

constexpr size_t SKYFISH_HITBOX_COUNT = static_cast<size_t>(SkyFishHitboxIndex::Count);

static_assert(SKYFISH_HITBOX_COUNT == ENEMY_HITBOX_COUNT_MAX);


}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_SKY_FISH_HITBOX_INDEX_HPP*/

