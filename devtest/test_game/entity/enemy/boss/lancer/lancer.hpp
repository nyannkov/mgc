#ifndef MGC_ENEMY_LANCER_HPP
#define MGC_ENEMY_LANCER_HPP

#include "app_common.hpp"
#include "entity/enemy/enemy.hpp"
#include "entity/attack/attack_lance/attack_lance.hpp"
#include "lancer_anim.hpp"

namespace app {
namespace enemy {

constexpr int32_t LancerMaxHP = 30;

struct Lancer: Enemy {

    Lancer(
        const FrameTimerT& timer,
        SoundControllerT& sound,
        const mgc::features::HasPosition<mgc::math::Vec2i>& target_pos
    );
    ~Lancer() = default;
    Lancer(const Lancer&) = delete;
    Lancer& operator=(const Lancer&) = delete;
    Lancer(Lancer&&) = default;
    Lancer& operator=(Lancer&&) = default;

    void spawn(const mgc::math::Vec2i& pos, bool is_right) override;
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    int32_t apply_damage_to(
        Player& player,
        size_t enemy_hitbox_index
    ) const override;
    void receive_damage(int32_t amount) override;
    void receive_impact(mgc::math::Vec2f delta, float dumping_rate) override;
    
    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) override;

    void set_enabled_hitbox_body(bool enabled);
    void fight() { fight_state_ = FightState::Fight; }
    void wait() { fight_state_ = FightState::Wait; }
    void won() { fight_state_ = FightState::Victory; }

    virtual ArrayViewer<attack::Attack*> weapons() { return {weapons_.data(), weapons_.size()}; }

private:
    enum class ActionState {
        Wait,
        Victory,
        Walking,
        ReadyToThrow,
        Launch,
        Throwing,
        Thrown
    } action_ = ActionState::Wait;
    const mgc::features::HasPosition<mgc::math::Vec2i>& target_pos_;
    SoundControllerT& sound_;
    AnimControllerT anim_;
    mgc::math::Vec2f force_ex_;
    mgc::math::Vec2f velocity_;
    LancerAnimState anim_state_ = LancerAnimState::WalkLeft;
    LancerAnimState prev_anim_state_ = LancerAnimState::WalkLeft;
    uint32_t update_bgm_param_count_ = 0;
    BlinkAnimatorT blink_animator_;
    StopwatchT sw_;
    std::array<attack::AttackLance, 3> lance_;
    std::array<attack::Attack*, 3> weapons_;
    void spawn_lance(
            const mgc::math::Vec2i& pos,
            attack::AttackLance& lance,
            attack::AttackLanceType type
    );
    enum class FightState {
        Wait,
        Fight,
        Victory
    } fight_state_ = FightState::Wait;
};

}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_LANCER_HPP*/

