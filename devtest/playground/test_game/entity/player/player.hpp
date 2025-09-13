#ifndef MGC_PLAYER_HPP
#define MGC_PLAYER_HPP

#include "app_common.hpp"
#include "entity/stage/collision_tile_layer/collision_tile_layer.hpp"
#include "entity/attack/scratch/scratch.hpp"
#include "entity/enemy/enemy_state.hpp"
#include "entity/prop/prop.hpp"
#include "player_anim.hpp"
#include "player_hitbox_index.hpp"

namespace app {
struct Player : mgc::entities::ActorImpl<Player, static_cast<size_t>(PlayerHitboxIndex::Count)> {

    Player(const FrameTimerT& frame_timer, const GamepadT& gamepad, SoundControllerT& sound_controller);
    ~Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    void spawn(const mgc::math::Vec2i& pos, PlayerAnimState anim_state);
    void pre_update();
    void post_update();
    Attack& attack() { return attack_; }

    void receive_damage(int32_t amount);
    void receive_heal(int32_t amount);
    void receive_life_up(int32_t amount);
    void receive_impact(mgc::math::Vec2f delta);

    int32_t hp() const { return hp_; }
    int32_t full_hp() const { return full_hp_; }

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

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, enemy::Enemy>) {
            if ( other.enemy_state() == enemy::EnemyState::Active ) {
                on_enemy_hit(other, info);
            }
        } else if constexpr (std::is_same_v<Other, item::Item>) {
            on_item_hit(other, info);
        }
    }

    // TODO
    bool is_game_over() const { return is_game_over_; }
    void clear_game_over() { is_game_over_ = false; }
    void set_game_over();
    void set_power_pose_in_game_over() { anim_state_ = PlayerAnimState::PowerPose; }
    void set_void_pose_in_game_over() { anim_state_ = PlayerAnimState::VoidPose; }
    void revive();

private:
    const GamepadT& gamepad_;
    const FrameTimerT& frame_timer_;
    SoundControllerT& sound_controller_;
    mgc::control::anim::AnimController<FrameTimerT> anim_;
    mgc::math::Vec2f velocity_;
    bool is_grounded_;
    bool is_grounded_pre_;
    PlayerAnimState anim_state_;
    bool is_right_;
    int32_t hp_;
    int32_t full_hp_;
    bool is_invulnerable_;
    enum class AttackState {
        Stop, Start, InProgress
    } attack_state_;
    Scratch attack_;
    BlinkAnimatorT blink_animator_;
    mgc::math::Vec2f force_ex_;
    bool is_game_over_;

    void set_hp(int32_t hp) { hp_ = hp; };
    void set_full_hp(int32_t full_hp) { full_hp_ = full_hp; };
    void update_anim_normal();
    void update_anim_attacking();
    void update_anim_game_over();
    void on_enemy_hit(
        const enemy::Enemy& enemy,
        const mgc::collision::BoxCollisionInfo& info
    );
    void on_item_hit(
        const item::Item& item,
        const mgc::collision::BoxCollisionInfo& info
    );
    void on_collision_resolved(
        const CollisionTileLayer& layer,
        const mgc::collision::MapPushbackInfo& info
    );
};


}// namespace app


#endif/*MGC_PLAYER_HPP*/

