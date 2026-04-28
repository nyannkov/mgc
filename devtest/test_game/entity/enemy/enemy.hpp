#ifndef MGC_ENEMY_HPP
#define MGC_ENEMY_HPP

#include "mgc_cpp/mgc.hpp"
#include "enemy_state.hpp"
#include "entity/stage/layer/layer_block.hpp"
#include "entity/stage/layer/layer_water.hpp"
#include "app_common.hpp"
#include "entity/player/player_hitbox_index.hpp"
#include "enemy_hitbox_index.hpp"

namespace app {
namespace enemy {

struct Enemy : mgc::entities::ActorImpl<
        Enemy, static_cast<size_t>(EnemyHitboxIndex::Count)
    > {

    virtual ~Enemy() = default;

    virtual void spawn(const mgc::math::Vec2i& pos, bool is_right) = 0;
    virtual void despawn() = 0;
    virtual void update_movement() = 0;
    virtual void update_animation() = 0;

    virtual int32_t apply_damage_to(Player& player, size_t hitbox_index) const = 0;
    virtual void receive_damage(int32_t amount) = 0;
    virtual void receive_impact(mgc::math::Vec2f delta) = 0;

    int32_t hp() const { return hp_; }
    int32_t full_hp() const { return full_hp_; }
    EnemyState enemy_state() const { return enemy_state_; }
    int32_t money() const { return money_amount_; }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if ( enemy_state_ == EnemyState::Active ) {
            if constexpr (std::is_same_v<Other, Player>) {
                if ( info.other_hitbox_index == 
                    static_cast<size_t>(PlayerHitboxIndex::Body) 
                ) {
                    on_player_hit(other, info);
                }
            } else if constexpr (std::is_same_v<Other, attack::Attack>) {
                
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
        if constexpr (std::is_same_v<MapT, stage::LayerBlock>) {
            on_collision_resolved(map, info);
        } else if constexpr (std::is_same_v<MapT, stage::LayerWater>) {
            on_collision_resolved(map, info);
        }
    }

protected:
    void set_enemy_state(EnemyState state) { enemy_state_ = state; }
    void set_hp(int32_t hp) { hp_ = hp; }
    void set_full_hp(int32_t full_hp) { full_hp_ = full_hp; };
    void set_money(int32_t amount) { money_amount_ = amount; }

    virtual void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_enemy_hit(
        const Enemy& enemy,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) { }

    virtual void on_collision_resolved(
        const stage::LayerWater& water,
        const mgc::collision::MapPushbackInfo& info
    ) { }

private:
    EnemyState enemy_state_ = EnemyState::Inactive;
    int32_t hp_ = 0;
    int32_t full_hp_ = 0;
    int32_t money_amount_ = 0;
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_HPP*/

