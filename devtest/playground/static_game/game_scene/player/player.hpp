#ifndef MGC_PLAYER_HPP
#define MGC_PLAYER_HPP

#include "app_typedefs.hpp"
#include "game_context.hpp"
#include "resources/generated/tileset/tileset_player.h"
#include "resources/generated/btree/test_btree.h"

namespace app {

struct Stage;
struct Enemy;

struct Player : mgc::entities::ActorImpl<Player, 1>,
                mgc::features::Updatable<GameContext> {

    Player() : heal_cooldown_(0),
               damage_cooldown_(0),
               life_value_(10),
               vy_(0),
               is_enemy_hit_(false),
               jumping_(false),
               bt_controller_() {

        this->sprite().set_tileset(tileset_player);
        this->sprite().set_tile_index(3);
        auto& hitboxes = this->hitboxes();
        hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
        hitboxes[0].size = mgc::collision::HitboxSize(16, 16);
        hitboxes[0].enabled = true;

        this->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(38)));

        bt_controller_.set_btree(test_btree);
        bt_controller_.bind_listener(bt_listener_);
    }
    ~Player() = default;

    const size_t& life_value() const {
        return life_value_;
    }

    void update(GameContext& ctx) override {

        using mgc::platform::input::Key;
        auto& gamepad = ctx.button();
        auto pos = this->position();

        if ( gamepad.is_pressed(Key::Left) ) {
             pos.x -= 4;
        } else if ( gamepad.is_pressed(Key::Right) ) {
             pos.x += 4;
        } else { }

        // Jump logic with simple vertical velocity (vy_)
        if ( gamepad.just_pressed(Key::Cancel) ) {
            if ( !jumping_ ) {
                jumping_ = true;
                vy_ = -12;
            }
        }
        
        pos.y += vy_;
        if ( vy_ < MGC_CELL_LEN ) {
            vy_++;
        }

        this->set_position(pos);

        if ( gamepad.just_pressed(Key::Control) ) {
            ctx.sound().play_sound_effect(0, 0.0); // Meow
        }

        if ( bt_controller_.has_finished() ) {
            bt_controller_.reset_state();
        }
        bt_controller_.proc_until_blocked(ctx.is_any_key_pressed());

        auto anim_state = bt_listener_.anim_state();
        bool is_sleep = false;

        if ( anim_state == AnimState::Dance ) {
            this->sprite().set_tile_index(21);
        } else if ( anim_state == AnimState::Sleep ) {
            is_sleep = true;
            this->sprite().set_tile_index(20);
        } else {
            this->sprite().set_tile_index(3);
        }

        if ( is_sleep ) {
            if ( heal_cooldown_ == 0 ) {
                this->inclement_life();
                heal_cooldown_ = HEAL_INTERVAL;
            } else {
                heal_cooldown_--;
            }
        } else {
            heal_cooldown_ = HEAL_INTERVAL;
        }

        if ( is_enemy_hit_ ) {
            if ( damage_cooldown_ == 0 ) {
                this->declement_life();
                damage_cooldown_ = DAMAGE_INTERVAL;
            } else {
                damage_cooldown_--;
            }
        } else {
            damage_cooldown_ = DAMAGE_INTERVAL;
        }

        is_enemy_hit_ = false;

    }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 

        if constexpr (std::is_same_v<Other, Enemy>) {
            if ( info.other_hitbox.id  == 0 ) { // TODO 0 == Body
                is_enemy_hit_ = true;
            }
        }
    }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        if constexpr (std::is_same_v<MapT, Stage>) {

            auto pos = this->position();
            // Pushback response logic: stop falling or bounce depending on direction
            if ( info.pushback.y < 0 ) {
                vy_ = 0;
                pos.y += info.pushback.y;
                jumping_ = false;
            } else if ( info.pushback.y > 0 ) {
                vy_ *= -1;
                pos.y += info.pushback.y;
            } else { }
            pos.x += info.pushback.x;

            this->set_position(pos);
        }
    }

private:
    static constexpr size_t HEAL_INTERVAL = 100;
    static constexpr size_t DAMAGE_INTERVAL = 30;
    static constexpr size_t MAX_LIFE = 48;
    size_t heal_cooldown_;
    size_t damage_cooldown_;
    size_t life_value_;

    void inclement_life() {
        if ( life_value_ < MAX_LIFE ) {
            life_value_++;
        }
    }

    void declement_life() {
        if ( 0 < life_value_ ) {
            life_value_--;
        }
    }

    int16_t vy_;
    bool is_enemy_hit_;
    bool jumping_;
    BTreeControllerT bt_controller_;
    enum class AnimState {
        Normal = 0,
        Dance,
        Sleep
    };

    struct BTreeListener : IBTreeListenerT {
        using LeafResult = typename IBTreeListenerT::LeafResult;
        using DurationT  = typename IBTreeListenerT::DurationT;

        AnimState anim_state() const {
            return anim_state_;
        }

        LeafResult on_proc_leaf(
            std::string_view id,
            const DurationT& duration,
            mgc_btree_tag_t tag
        ) override  {

            if ( id == "cond/timer/over_60s" ) {

                if ( duration.input_idle_time > 60'000 ) {
                    return LeafResult::Success;
                } else {
                    return LeafResult::Failure;
                }

            } else if ( id == "action/sleep" ) {

                anim_state_ = AnimState::Sleep;
                return LeafResult::Success;

            } else if ( id == "cond/timer/over_30s" ) {

                if ( duration.input_idle_time > 30'000 ) {
                    return LeafResult::Success;
                } else {
                    return LeafResult::Failure;
                }

            } else if ( id == "action/dance" ) {
                
                anim_state_ = AnimState::Dance;
                return LeafResult::Success;

            } else if ( id == "idle_breathing" ) {

                anim_state_ = AnimState::Normal;

                return LeafResult::Success;

            } else {

                return LeafResult::Running;
            }
        }

    private:
        AnimState anim_state_ = AnimState::Normal;
    } bt_listener_;
};


}// namespace app


#endif/*MGC_PLAYER_HPP*/

