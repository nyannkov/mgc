#ifndef MGC_ENEMY_HPP
#define MGC_ENEMY_HPP

#include "mgc_cpp/mgc.hpp"
#include "enemy_state.hpp"

namespace app {

struct Player;
struct Attack;
struct CollisionTileLayer;

namespace enemy {

constexpr size_t ENEMY_HITBOX_COUNT_MAX = 3;

struct Enemy : mgc::entities::ActorImpl<Enemy, ENEMY_HITBOX_COUNT_MAX> {

    virtual ~Enemy() = default;

    virtual void spawn(const mgc::math::Vec2i& pos, bool is_right) = 0;
    virtual void despawn() = 0;
    virtual void pre_update() = 0;
    virtual void post_update() = 0;

    void take_damage(int32_t amount) {
        hp_ = ( hp_ > amount ) ? (hp_ - amount) : 0;
    }
    void set_hp(int32_t hp) { hp_ = hp; };
    int32_t hp() const { return hp_; }
    void set_enemy_state(EnemyState state) { enemy_state_ = state; }
    EnemyState enemy_state() const { return enemy_state_; }

    virtual void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_attack_hit(
        const Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_enemy_hit(
        const Enemy& enemy,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_collision_resolved(
        const CollisionTileLayer& layer,
        const mgc::collision::MapPushbackInfo& info
    ) { }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if ( enemy_state_ == EnemyState::Active ) {
            if constexpr (std::is_same_v<Other, Player>) {
                
                on_player_hit(other, info);

            } else if constexpr (std::is_same_v<Other, Attack>) {
                
                on_attack_hit(other, info);

            } else if constexpr (std::is_same_v<Other, Enemy>) {

                on_enemy_hit(other, info);

            } else { }
        }
    }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        if constexpr (std::is_same_v<MapT, CollisionTileLayer>) {
            
            on_collision_resolved(map, info);
        }
    }

private:
    EnemyState enemy_state_ = EnemyState::Inactive;
    int32_t hp_ = 0;
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_HPP*/

