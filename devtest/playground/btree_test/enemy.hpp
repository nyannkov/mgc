#ifndef MGC_ENEMY_HPP
#define MGC_ENEMY_HPP

#include <cstdlib>
#include "mgc_cpp/math/vec2.hpp"
#include "platform.hpp"
#include "resources/generated/tileset/tileset_fish.h"
#include "resources/generated/btree/btree_chase.h"

namespace devtest {

struct Stage;

template <typename PlatformT>
struct Enemy : mgc::entities::ActorImpl<Enemy<PlatformT>, 3> {

    enum class EnemyType {
        SkyFish
    };

    enum class HitboxId : mgc_id_t {
        Body = 0,
        ViewLeft = 1,
        ViewRight = 2
    };

    using TimerT = typename PlatformT::TimerT;
    using BTreeControllerT = mgc::control::btree::BTreeController<TimerT>;
    using IBTreeListenerT = mgc::control::btree::IBTreeListener<BTreeControllerT>;

    explicit Enemy(PlatformT& pf) 
            : pf_(pf), 
              bt_controller_(),
              bt_listener_(player_in_view_) {

        set_by_id(EnemyType::SkyFish);
        bt_controller_.bind_listener(bt_listener_);
    }
    ~Enemy() = default;

    void set_by_id(EnemyType type) {
        if ( type == EnemyType::SkyFish ) {
            player_in_view_ = false;
            this->sprite().set_tileset(tileset_fish);
            this->sprite().set_tile_index(4);
            auto& hitboxes = this->hitboxes();
            // body
            hitboxes[0].id = static_cast<mgc_id_t>(HitboxId::Body);
            hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
            hitboxes[0].size = mgc::collision::HitboxSize(16, 16);
            hitboxes[0].enabled = true;

            // view left
            hitboxes[1].id = static_cast<mgc_id_t>(HitboxId::ViewLeft);
            hitboxes[1].offset = mgc::collision::HitboxOffset(8-16*7, 8-16*5);
            hitboxes[1].size = mgc::collision::HitboxSize(16*7, 16*10);
            hitboxes[1].enabled = true;

            // view right
            hitboxes[2].id = static_cast<mgc_id_t>(HitboxId::ViewRight);
            hitboxes[2].offset = mgc::collision::HitboxOffset(8, 8-16*5);
            hitboxes[2].size = mgc::collision::HitboxSize(16*7, 16*10);
            hitboxes[2].enabled = false;

            this->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(35)));

            this->temp_position_ = this->position().template cast_to<float>();
            this->velocity_ = mgc::math::Vec2<float>(0.0f, 0.0f);

            bt_controller_.set_btree(btree_chase);
        }
    }

    void update(mgc::math::Vec2<float> player_pos) {

        auto enemy_state = bt_listener_.enemy_state();
        auto& hitboxes = this->hitboxes();

        if ( enemy_state == BTreeListener::EnemyState::Chase ) {

            const float stiffness = 0.03f;
            const float damping = 1.0f;

            auto delta = player_pos - temp_position_;
            auto force = delta * stiffness - velocity_ * damping;
            velocity_ = velocity_ + force;
            temp_position_ = temp_position_ + velocity_;

            if ( this->temp_position_.x < player_pos.x ) {
                this->sprite().set_tile_index(1);
            } else {
                this->sprite().set_tile_index(4);
            }
            
            this->set_position(temp_position_.template cast_to<int16_t>());

        } else if ( enemy_state == BTreeListener::EnemyState::LookLeft ) {
            hitboxes[1].enabled = true;
            hitboxes[2].enabled = false;
            this->sprite().set_tile_index(4);

        } else if ( enemy_state == BTreeListener::EnemyState::LookRight ) {
            hitboxes[1].enabled = false;
            hitboxes[2].enabled = true;
            this->sprite().set_tile_index(1);

        } else {

        }


        if ( bt_controller_.has_finished() ) {
            bt_controller_.reset_state();
        }

        bt_controller_.proc_until_blocked(mgc::platform::input::is_any_button_pressed(pf_.input()));

        player_in_view_ = false;
    }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, Player<PlatformT>>) {
            switch ( info.self_hitbox.id ) {
            case static_cast<mgc_id_t>(HitboxId::ViewLeft):/*fallthrough*/
            case static_cast<mgc_id_t>(HitboxId::ViewRight):
                player_in_view_ = true;
                break;
            default:
                break;
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
        }
    }

private:
    mgc::math::Vec2<float> temp_position_;
    mgc::math::Vec2<float> velocity_;
    PlatformT& pf_;
    BTreeControllerT bt_controller_;
    bool player_in_view_;

    struct BTreeListener : IBTreeListenerT {
        enum class EnemyState {
            LookLeft,
            LookRight,
            Chase
        };
        using LeafResult = typename IBTreeListenerT::LeafResult;
        using DurationT  = typename IBTreeListenerT::DurationT;

        explicit BTreeListener(const bool& player_in_view) 
            : in_view_(player_in_view),
              count_(0),
              enemy_state_(EnemyState::LookLeft) { }


        LeafResult on_proc_leaf(
            std::string_view id,
            const DurationT& duration,
            mgc_btree_tag_t tag
        ) override  {

            if ( id == "cond/player/visible" ) {
                
                if ( in_view_ ) {
                    return LeafResult::Success;
                } else {
                    return LeafResult::Failure;
                }

            } else if ( id == "action/player/chase" ) {

                enemy_state_ = EnemyState::Chase;
                return LeafResult::Success;

            } else if ( id == "action/look/random" ) {

                if ( ++count_ >= 30 ) {
                    count_ = 0;
                    if ( ( rand() % 2 ) == 0 ) {
                        enemy_state_ = EnemyState::LookLeft;
                    } else {
                        enemy_state_ = EnemyState::LookRight;
                    }
                } else {
                    if ( enemy_state_ == EnemyState::Chase ) {
                        // Temporary hardcorded value
                        enemy_state_ = EnemyState::LookLeft;
                    }
                }
                return LeafResult::Success;

            } else {

                return LeafResult::Failure;
            }
        }

        EnemyState enemy_state() const {
            return enemy_state_;
        }

    private:
        EnemyState enemy_state_;
        const bool& in_view_;
        uint32_t count_;

    } bt_listener_;
};


}// namespace devtest


#endif/*MGC_ENEMY_HPP*/

