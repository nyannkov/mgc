#ifndef MGC_ATTACK_PLAYER_HPP
#define MGC_ATTACK_PLAYER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/attack/attack.hpp"

namespace app {
namespace attack {

enum class AttackPlayerType {
    Scratch,
    Boomerang,
    Yoyo
};

struct AttackPlayer : Attack {

    AttackPlayer(
        const FrameTimerT& frame_timer, 
        const GamepadT& gamepad,
        SoundControllerT& sound,
        const mgc::features::HasPosition<mgc::math::Vec2i>& owner_pos
    );

    void spawn(
        const mgc::math::Vec2i& pos,
        AttackOwner owner
    ) override;

    void spawn(
        const mgc::math::Vec2i& pos,
        AttackPlayerType type,
        AttackOwner owner,
        AttackDirection dir
    );
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    int32_t apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const override;
    int32_t apply_damage_to(Player& player, size_t attack_hitbox_index) const override;

    void draw_wrap(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override;

    auto attack_type() const { return attack_type_; }

private:
    AnimControllerT anim_;
    const GamepadT& gamepad_;
    SoundControllerT& sound_;
    mgc::math::Vec2f velocity_;
    mgc::math::Vec2i pos_orig_;
    bool hit_ = false;//TODO
    const mgc::features::HasPosition<mgc::math::Vec2i> &owner_pos_;
    mgc::math::Vec2f yoyo_sum_i_;
    AttackPlayerType attack_type_ = AttackPlayerType::Scratch;

    AttackOwner owner_type_ = AttackOwner::Player;
    void set_attack_type(AttackPlayerType type) { attack_type_ = type; }

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

    void spawn_yoyo(
        const mgc::math::Vec2i& pos,
        AttackOwner owner,
        AttackDirection dir
    );
    void despawn_yoyo();
    void update_animation_yoyo();
    void update_movement_yoyo();

    void on_enemy_hit(
        const enemy::Enemy& enemy,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

};

}// namespace attack
}// namespace app

#endif/*MGC_ATTACK_PLAYER_HPP*/

