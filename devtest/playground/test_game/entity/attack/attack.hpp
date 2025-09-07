#ifndef MGC_ATTACK_HPP
#define MGC_ATTACK_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"

namespace app {

enum class AttackOwner {
    Player,
    Enemy
};

enum class AttackDirection {
    Right,
    Left
};

constexpr size_t ATTACK_HITBOX_COUNT_MAX = 2;

struct Attack : mgc::entities::ActorImpl<Attack, ATTACK_HITBOX_COUNT_MAX> {

    virtual ~Attack() = default;

    virtual void spawn(const mgc::math::Vec2i& pos, AttackOwner owner, AttackDirection dir) = 0;
    virtual void despawn() = 0;

    virtual void pre_update() { }
    virtual void post_update() { }

    AttackDirection direction() const { return direction_; }
    void set_damage(int32_t damage) { damage_ = damage; }
    int32_t damage() const { return damage_; }
    void set_owner_type(AttackOwner owner) { owner_type_ = owner; }
    AttackOwner owner_type() const { return owner_type_; }

protected:
    void set_direction(AttackDirection dir) { direction_ = dir; }
    
private:
    int32_t damage_;
    AttackOwner owner_type_ = AttackOwner::Player;
    AttackDirection direction_ = AttackDirection::Right;
};


}// namespace app

#endif/*MGC_ATTACK_HPP*/

