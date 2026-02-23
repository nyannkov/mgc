#ifndef MGC_ATTACK_HPP
#define MGC_ATTACK_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"

namespace app {
namespace attack {

enum class AttackType {
    Scratch,
    Boomerang,
};

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
    Left
};

enum class AttackHitboxIndex : size_t {
    Body = 0,
    Count
};

struct Attack : mgc::entities::ActorImpl<
        Attack, static_cast<size_t>(AttackHitboxIndex::Count)
    > {

    Attack(
        const FrameTimerT& frame_timer, 
        const GamepadT& gamepad,
        SoundControllerT& sound
    );
    ~Attack() = default;

    void spawn(
        const mgc::math::Vec2i& pos,
        AttackType type,
        AttackOwner owner,
        AttackDirection dir
    );
    void despawn();
    void update_movement();
    void update_animation();

    int32_t apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const;
    int32_t apply_damage_to(Player& player, size_t attack_hitbox_index) const;

    AttackLifeCycle lifecycle() const { return lifecycle_; }
    AttackOwner owner_type() const { return owner_type_; }
    AttackDirection direction() const { return direction_; }
    AttackType attack_type() const { return attack_type_; }
    mgc::math::Vec2f velocity() const { return velocity_; }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { }

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

private:
    AnimControllerT anim_;
    const GamepadT& gamepad_;
    SoundControllerT& sound_;
    mgc::math::Vec2f velocity_;
    mgc::math::Vec2i pos_orig_;

    AttackOwner owner_type_ = AttackOwner::Player;
    AttackType attack_type_ = AttackType::Scratch;
    AttackDirection direction_ = AttackDirection::Right;
    AttackLifeCycle lifecycle_ = AttackLifeCycle::Despawned;

    void set_direction(AttackDirection dir) { direction_ = dir; }
    void set_lifecycle(AttackLifeCycle lifecycle) { lifecycle_ = lifecycle; }
    void set_owner_type(AttackOwner owner) { owner_type_ = owner; }
    void set_attack_type(AttackType type) { attack_type_ = type; }
    void set_velocity(mgc::math::Vec2f v) { velocity_ = v; }


    void spawn_scratch(
        const mgc::math::Vec2i& pos,
        AttackOwner owner,
        AttackDirection dir
    );
    void despawn_scratch();
    void update_animation_scratch();
    void update_movement_scratch();

    void spawn_boomerang(
        const mgc::math::Vec2i& pos,
        AttackOwner owner,
        AttackDirection dir
    );
    void despawn_boomerang();
    void update_animation_boomerang();
    void update_movement_boomerang();

};

}// namespace attack
}// namespace app

#endif/*MGC_ATTACK_HPP*/

