#ifndef MGC_ATTACK_HPP
#define MGC_ATTACK_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/player/player_hitbox_index.hpp"

namespace app {
namespace attack {

enum class AttackLifeCycle {
    Despawned,
    Spawing,
    Spawned,
    Despawning
};

enum class AttackOwner {
    Player,
    Enemy
};

enum class AttackDirection {
    Right,
    Left,
    UpRight,
    UpLeft
};

enum class AttackHitboxIndex : size_t {
    Body = 0,
    Count
};

struct Attack : mgc::entities::ActorImpl<
        Attack, static_cast<size_t>(AttackHitboxIndex::Count)
    > {

    virtual ~Attack() = default;

    virtual void spawn(
        const mgc::math::Vec2i& pos,
        AttackOwner owner
    ) = 0;
    virtual void despawn() = 0;
    virtual void update_movement() = 0;
    virtual void update_animation() = 0;

    virtual int32_t apply_damage_to(
        enemy::Enemy& enemy,
        size_t attack_hitbox_index
    ) const = 0;
    virtual int32_t apply_damage_to(
        Player& player,
        size_t attack_hitbox_index
    ) const = 0;

    AttackLifeCycle lifecycle() const { return lifecycle_; }
    AttackOwner owner_type() const { return owner_type_; }
    AttackDirection direction() const { return direction_; }
    mgc::math::Vec2f velocity() const { return velocity_; }

    virtual void draw_wrap(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) {
        this->draw(fb, cam_pos);
    }

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
        } else if constexpr (std::is_same_v<Other, enemy::Enemy>) {

            on_enemy_hit(other, info);

        } else { }
    }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { }

    template <typename View>
    void handle_box_pushback_result_impl(                                              
            const mgc::collision::BoxPushbackInfo& info,                               
            const View& others
    ) { }   

protected:
    void set_direction(AttackDirection dir) { direction_ = dir; }
    void set_lifecycle(AttackLifeCycle lifecycle) { lifecycle_ = lifecycle; }
    void set_owner_type(AttackOwner owner) { owner_type_ = owner; }
    void set_velocity(mgc::math::Vec2f v) { velocity_ = v; }
    virtual void on_enemy_hit(
        const enemy::Enemy& enemy,
        const mgc::collision::BoxCollisionInfo& info
    ) { }
    virtual void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

private:
    mgc::math::Vec2f velocity_;
    AttackOwner owner_type_ = AttackOwner::Player;
    AttackDirection direction_ = AttackDirection::Right;
    AttackLifeCycle lifecycle_ = AttackLifeCycle::Despawned;
};

}// namespace attack
}// namespace app

#endif/*MGC_ATTACK_HPP*/

