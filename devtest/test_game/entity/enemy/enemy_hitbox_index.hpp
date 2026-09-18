#ifndef MGC_ENEMY_HITBOX_INDEX_HPP
#define MGC_ENEMY_HITBOX_INDEX_HPP

namespace app {
namespace enemy {

enum class EnemyHitboxIndex : size_t {
    Body = 0,
    ViewLeft,
    ViewRight,
    Core,
    Head,
    ViewBottom,
    Count
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_HITBOX_INDEX_HPP*/

