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
    Yoyo,
    Firework,
    Firework_Bursting
};

enum class FireworkPhase {
    Idle,
    Burning,
    Detonating,
    Bursting,
    Bursting_2,
    Fading,
};

enum class HoldState {
    Off,
    On
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
        AttackDirection dir,
        const mgc::math::Vec2f& velocity
    );
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    int32_t apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const override;
    int32_t apply_damage_to(Player& player, size_t attack_hitbox_index) const override;

    bool can_break_block() const override;

    void draw_wrap(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override;

    auto attack_type() const { return attack_type_; }
    void launch(const mgc::math::Vec2f& velocity);
    auto firework_phase() const { return firework_phase_; }
    auto hold_state() const { return hold_state_; }

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
    FireworkPhase firework_phase_ = FireworkPhase::Idle;
    HoldState hold_state_ = HoldState::Off;
    StopwatchT sw_;
    mgc::graphics::DrawOptions draw_opt_;
    mgc_color_t color_palette_[32];
    mgc_color_t color_mask_ = 0;

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

    void spawn_firework(
        const mgc::math::Vec2i& pos,
        AttackOwner owner,
        AttackDirection dir
    );
    void despawn_firework();
    void update_animation_firework();
    void update_movement_firework();

    void spawn_firework_bursting(
        const mgc::math::Vec2i& pos,
        AttackOwner owner,
        AttackDirection dir
    );
    void despawn_firework_bursting();
    void update_animation_firework_bursting();
    void update_movement_firework_bursting();

    void on_enemy_hit(
        const enemy::Enemy& enemy,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
    void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) override;
    void on_collision_resolved(
        const carrier::Carrier& carrier,
        const mgc::collision::MapPushbackInfo& info
    ) override;

};

}// namespace attack
}// namespace app

#endif/*MGC_ATTACK_PLAYER_HPP*/

