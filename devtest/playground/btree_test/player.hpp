#ifndef MGC_PLAYER_HPP
#define MGC_PLAYER_HPP

#include "platform.hpp"
#include "resources/generated/tileset/tileset_player.h"
#include "resources/generated/btree/test_btree.h"

namespace devtest {

struct Stage;

template <typename PlatformT>
struct Enemy;

template <typename PlatformT>
struct Player : mgc::entities::ActorImpl<Player<PlatformT>, 1> {

    using TimerT = typename PlatformT::TimerT;
    using BTreeControllerT = mgc::control::btree::BTreeController<TimerT>;
    using IBTreeListenerT = mgc::control::btree::IBTreeListener<BTreeControllerT>;

    explicit Player(PlatformT& pf) 
            : pf_(pf), 
              vy_(0),
              enemy_hit_(false),
              jumping_(false),
              bt_controller_(pf.input()) {

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

    bool is_sleep() const {
        return bt_listener_.anim_state() == AnimState::Sleep;
    }

    bool is_enemy_hit() const {
        return enemy_hit_;
    }

    void update() {
        using mgc::platform::input::Key;
        auto& gamepad = pf_.input();
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
            pf_.sound().play_sound_effect(0, 0.0); // Meow
        }

        if ( bt_controller_.has_finished() ) {
            bt_controller_.reset_state();
        }
        bt_controller_.proc_until_blocked();

        auto anim_state = bt_listener_.anim_state();

        if ( anim_state == AnimState::Dance ) {
            this->sprite().set_tile_index(21);
        } else if ( anim_state == AnimState::Sleep ) {
            this->sprite().set_tile_index(20);
        } else {
            this->sprite().set_tile_index(3);
        }

        enemy_hit_ = false;
    }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 

        if constexpr (std::is_same_v<Other, Enemy<PlatformT>>) {
            if ( info.other_hitbox.id  == 0 ) { // TODO 0 == Body
                enemy_hit_ = true;
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
    PlatformT& pf_;
    int16_t vy_;
    bool enemy_hit_;
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


}// namespace devtest


#endif/*MGC_PLAYER_HPP*/

