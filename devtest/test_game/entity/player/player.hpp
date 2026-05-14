#ifndef MGC_PLAYER_HPP
#define MGC_PLAYER_HPP

#include "app_common.hpp"
#include "entity/stage/layer/layer_block.hpp"
#include "entity/stage/layer/layer_ladder.hpp"
#include "entity/stage/layer/layer_one_way_block.hpp"
#include "entity/stage/layer/layer_needle.hpp"
#include "entity/stage/layer/layer_water.hpp"
#include "entity/attack/attack.hpp"
#include "entity/enemy/enemy_state.hpp"
#include "entity/prop/prop.hpp"
#include "entity/block/block.hpp"
#include "player_anim.hpp"
#include "player_hitbox_index.hpp"
#include "world_state/equipment_info.hpp"

namespace app {

enum class PlayerState {
    Normal = 0,
    Ladder,
    Swimming,
    Diving,
    DivingJump,
    GameOver
};

enum class PlayerAnimMode {
    Auto = 0,
    Manual
};

struct Player : mgc::entities::ActorImpl<Player, static_cast<size_t>(PlayerHitboxIndex::Count)> {

    Player(
        const FrameTimerT& frame_timer,
        const GamepadT& gamepad,
        SoundControllerT& sound_controller,
        EquipmentInfo& equipment_info
    );
    ~Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    void init();
    void spawn(const mgc::math::Vec2i& pos, PlayerAnimState anim_state);
    void update_movement();
    void resolve_movement();
    void update_animation(bool is_talking);
    attack::Attack& attack() { return attack_; }

    void reset_state_for_placement(
        const mgc::math::Vec2i& pos,
        PlayerAnimState anim_state
    );

    void receive_damage(int32_t amount);
    void receive_heal(int32_t amount);
    void receive_life_up(int32_t amount);
    void receive_impact(mgc::math::Vec2f delta);

    int32_t hp() const { return hp_; }
    int32_t full_hp() const { return full_hp_; }
    void add_money(int32_t amount) { 
        if ( amount < 0 ) {
            return;
        }
        if ( MAX_MONEY < amount ) {
            amount = MAX_MONEY;
        }

        if ( ( money_ + amount ) < 0 ) {
            money_ = 0;
        } else if ( MAX_MONEY < ( money_ + amount ) ) {
            money_ = MAX_MONEY;
        } else {
            money_ += amount; 
        }
    }
    void sub_money(int32_t amount) {
        if ( amount < 0 ) {
            return;
        }
        if ( MAX_MONEY < amount ) {
            amount = MAX_MONEY;
        }

        if ( ( money_ - amount ) < 0 ) {
            money_ = 0;
        } else if ( MAX_MONEY < ( money_ - amount ) ) {
            money_ = MAX_MONEY;
        } else {
            money_ -= amount; 
        }
    }
    void set_money(int32_t amount) { money_ = amount; }
    int32_t money() const { return money_; }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        if ( info.obj_hitbox_index == 
            static_cast<size_t>(PlayerHitboxIndex::Body) 
        ) {
            if constexpr (std::is_same_v<MapT, stage::LayerBlock>) {
                on_collision_resolved(map, info);
            } else if constexpr (std::is_same_v<MapT, stage::LayerLadder>) {
                hit_ladder_ = true;
            } else if constexpr (std::is_same_v<MapT, stage::LayerWater>) {
                hit_water_ = true;
            } else if constexpr (std::is_same_v<MapT, stage::LayerNeedle>) {
                on_collision_resolved(map, info);
            } else if constexpr (std::is_same_v<MapT, stage::LayerOneWayBlock>) {
                hit_one_way_block_ = true;
                on_collision_resolved(map, info);
            }
        } else if ( info.obj_hitbox_index == 
            static_cast<size_t>(PlayerHitboxIndex::Head) 
        ) {
            if constexpr (std::is_same_v<MapT, stage::LayerWater>) {
                hit_head_water_ = true;
            }
        }
    }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if ( info.self_hitbox_index == 
            static_cast<size_t>(PlayerHitboxIndex::Body) 
        ) {
            if constexpr (std::is_same_v<Other, enemy::Enemy>) {
                if ( other.enemy_state() == enemy::EnemyState::Active ) {
                    on_enemy_hit(other, info);
                }
            } else if constexpr (std::is_same_v<Other, item::Item>) {
                on_item_hit(other, info);
            }
        }
    }

    template <typename View>
    void handle_box_pushback_result_impl(                                              
            const mgc::collision::BoxPushbackInfo& info,                               
            const View& others
    ) { 
        if constexpr (std::is_same_v<View, mgc::collision::BoxBoxPtrArrayView<block::Block>>) {
            on_collision_resolved(info);
        }
    }   

    bool is_game_over() const { return player_state_ == PlayerState::GameOver; }
    void set_power_pose_in_game_over() { anim_state_ = PlayerAnimState::PowerPose; }
    void set_void_pose_in_game_over() { anim_state_ = PlayerAnimState::VoidPose; }
    void revive();
    bool is_ground() const { return is_grounded_; }
    
    void set_anim_mode(PlayerAnimMode mode) { anim_mode_ = mode; }
    auto anim_mode() const { return anim_mode_; }

    void set_anim_manually(PlayerAnimState state, bool loop = true);
    auto anim_state() const {
        if ( anim_mode_ == PlayerAnimMode::Auto ) {
            return anim_state_;
        } else {
            return anim_state_manual_;
        }
    }
    bool is_animation_finished() const {
        return anim_.is_finished();
    }

    void set_input_enabled(bool enabled) { input_enabled_ = enabled; }
    bool input_enabled() const { return input_enabled_; }
    void set_game_over();
    auto velocity() const { return velocity_; }

    auto& equipment_info() { return equipment_info_; }

private:
    const GamepadT& gamepad_;
    const FrameTimerT& frame_timer_;
    SoundControllerT& sound_controller_;
    EquipmentInfo& equipment_info_;
    mgc::control::anim::AnimController<FrameTimerT> anim_;
    mgc::math::Vec2f velocity_ {0.0f, 0.0f};
    mgc::math::Vec2f force_ex_ {0.0f, 0.0f};
    bool is_right_ = true;
    bool is_grounded_ = true;
    bool is_invulnerable_ = false;;
    bool hit_ladder_ = false;
    bool hit_water_ = false;
    bool hit_head_water_ = false;
    bool hit_one_way_block_ = false;
    bool one_way_block_falling_ = false;
    bool input_enabled_ = true;
    int32_t money_ = 100;
    PlayerAnimMode anim_mode_ = PlayerAnimMode::Auto;
    PlayerAnimState anim_state_ = PlayerAnimState::StandRight;
    PlayerAnimState anim_state_manual_ = PlayerAnimState::StandRight;
    PlayerState player_state_ = PlayerState::Normal;
    attack::AttackType current_attack_type_ = attack::AttackType::Boomerang;
    int32_t hp_;
    int32_t full_hp_;
    enum class AttackState {
        Stop, Start, InProgress
    } attack_state_ = AttackState::Stop;;
    attack::Attack attack_;
    BlinkAnimatorT blink_animator_;
    mgc::math::Vec2i pushback_box_ {};
    mgc::math::Vec2i pushback_map_ {};
    mgc::math::Vec2i box_overlap_ {};

    static constexpr int32_t MAX_MONEY = 99999;

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
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    );
    void on_collision_resolved(
        const stage::LayerOneWayBlock& block,
        const mgc::collision::MapPushbackInfo& info
    );
    void on_collision_resolved(
        const stage::LayerNeedle& block,
        const mgc::collision::MapPushbackInfo& info
    );
    void on_collision_resolved(
        const mgc::collision::BoxPushbackInfo& info
    );
};


}// namespace app


#endif/*MGC_PLAYER_HPP*/

