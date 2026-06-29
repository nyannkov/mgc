#ifndef MGC_ATTACK_LANCE_HPP
#define MGC_ATTACK_LANCE_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/attack/attack.hpp"

namespace app {
namespace attack {

enum class AttackLanceType {
    Hold,
    LeftwardAim,
    RightwardAim,
    UpwardAim,
    ThrowLeft,
    ThrowRight,
    ThrowUpLeft,
    ThrowUpRight,
    ThrowUp
};

struct AttackLance : Attack {

    AttackLance(
        const FrameTimerT& frame_timer, 
        SoundControllerT& sound
    );

    void spawn(
        const mgc::math::Vec2i& pos,
        AttackOwner owner
    ) override;
    void spawn(
        const mgc::math::Vec2i& pos,
        AttackLanceType type,
        AttackOwner owner
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
    void launch() { launch_state_ = LaunchState::Start; }

    void set_target(const mgc::features::HasPosition<mgc::math::Vec2i> *target) {
        target_ = target;
    }
    void set_owner(const mgc::features::HasPosition<mgc::math::Vec2i> *owner) {
        owner_ = owner;
    }
    void set_offset(const mgc::math::Vec2i& offset) {
        offset_ = offset;
    }

private:
    enum class LaunchState {
        Stop,
        Start,
        Launched
    } launch_state_ = LaunchState::Stop;
    enum class AnimType {
        Up,
        Down,
        Left,
        UpLeft,
        DownLeft,
        Right,
        UpRight,
        DownRight
    } anim_type_ = AnimType::Up;
    AnimControllerT anim_;
    SoundControllerT& sound_;
    mgc::math::Vec2f velocity_;
    AttackLanceType attack_type_ = AttackLanceType::Hold;
    AttackOwner owner_type_ = AttackOwner::Enemy;
    void set_attack_type(AttackLanceType type) { attack_type_ = type; }

    void on_enemy_hit(
        const enemy::Enemy& enemy,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
    const mgc::parts::assets::AnimFrames& get_anim_frames(AnimType type);
    const mgc::features::HasPosition<mgc::math::Vec2i> *target_ = nullptr;
    const mgc::features::HasPosition<mgc::math::Vec2i> *owner_= nullptr;
    mgc::math::Vec2i offset_{};
};

}// namespace attack
}// namespace app

#endif/*MGC_ATTACK_LANCE_HPP*/

