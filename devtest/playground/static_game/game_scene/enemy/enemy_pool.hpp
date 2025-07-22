#ifndef MGC_ENEMY_POOL_HPP
#define MGC_ENEMY_POOL_HPP

#include "enemy.hpp"

namespace app {

template <typename EnemyT>
struct EnemySlot {
    EnemyT enemy;
    bool active;
};

template <typename EnemyT, size_t N>
struct EnemyPool {
    std::array<EnemySlot<EnemyT>, N> enemy_slots;

    EnemyPool() {
        for ( auto& slot : enemy_slots ) {
            slot.active = false;
        }
    }

    EnemyT* spawn(mgc_id_t id) {
        
        for ( auto& slot : enemy_slots ) {
            if ( !slot.active ) {
                slot.active = true;
                slot.enemy.set_id(id);
                return &slot.enemy;
            }
        }

        return nullptr;
    }

    EnemyT* spawn(mgc_id_t id, EnemyKind kind) {
        for ( auto& slot : enemy_slots ) {
            if ( !slot.active ) {
                slot.active = true;
                slot.enemy.set_id(id);
                slot.enemy.setup(kind);
                return &slot.enemy;
            }
        }

        return nullptr;
    }

    EnemyT* get(mgc_id_t id) {
        for ( auto& slot : enemy_slots ) {
            if ( slot.active && slot.enemy.id() == id ) {
                return &slot.enemy;
            }
        }
        
        return nullptr;
    }

    void destroy(EnemyT* e) {
        if ( !e ) return;

        for ( auto& slot : enemy_slots ) {
            if ( &slot.enemy == e ) {
                slot.active = false;
                break;
            }
        }
    }

    void destroy_all() {
        for ( auto& slot : enemy_slots ) {
            slot.active = false;
        }
    }
};

using EnemyPoolT = EnemyPool<Enemy, 10>;

}// namespace app


#endif/*MGC_ENEMY_POOL_HPP*/

