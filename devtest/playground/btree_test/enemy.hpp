#ifndef MGC_ENEMY_HPP
#define MGC_ENEMY_HPP

#include "mgc_cpp/math/vec2.hpp"
#include "platform.hpp"
#include "resources/generated/tileset/tileset_fish.h"

namespace devtest {

struct Stage;

template <typename PlatformT>
struct Enemy : mgc::entities::ActorImpl<Player<PlatformT>, 1> {

    using TimerT = typename PlatformT::TimerT;
    using BTreeControllerT = mgc::control::btree::BTreeController<TimerT>;
    using IBTreeListenerT = mgc::control::btree::IBTreeListener<BTreeControllerT>;

    explicit Enemy(PlatformT& pf) 
            : pf_(pf), 
              bt_controller_(pf.input()) {

        this->sprite().set_tileset(tileset_fish);
        this->sprite().set_tile_index(4);
        auto& hitboxes = this->hitboxes();
        hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
        hitboxes[0].size = mgc::collision::HitboxSize(16, 16);
        hitboxes[0].enabled = true;

        this->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(35)));

        this->temp_position_ = this->position().template cast_to<float>();
        this->velocity_ = mgc::math::Vec2<float>(0.0f, 0.0f);

        //bt_controller_.set_btree(test_btree);
        //bt_controller_.bind_listener(bt_listener_);
    }
    ~Enemy() = default;

    void update(mgc::math::Vec2<float> player_pos) {

        const float stiffness = 0.03f;
        const float damping = 1.0f;

        auto delta = player_pos - temp_position_;
        auto force = delta * stiffness - velocity_ * damping;
        velocity_ = velocity_ + force;
        temp_position_ = temp_position_ + velocity_;
        
        this->set_position(temp_position_.template cast_to<int16_t>());

        //if ( bt_controller_.has_finished() ) {
        //    bt_controller_.reset_state();
        //}
        //bt_controller_.proc_until_blocked();

        //auto anim_state = bt_listener_.anim_state();

        //if ( anim_state == AnimState::Dance ) {
        //    this->sprite().set_tile_index(21);
        //} else if ( anim_state == AnimState::Sleep ) {
        //    this->sprite().set_tile_index(20);
        //} else {
        //    this->sprite().set_tile_index(3);
        //}
    }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { }

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
    //enum class AnimState {
    //    Normal = 0,
    //    Dance,
    //    Sleep
    //};

    //struct BTreeListener : IBTreeListenerT {
    //    using LeafResult = typename IBTreeListenerT::LeafResult;
    //    using DurationT  = typename IBTreeListenerT::DurationT;

    //    AnimState anim_state() const {
    //        return anim_state_;
    //    }

    //    LeafResult on_proc_leaf(
    //        std::string_view id,
    //        const DurationT& duration,
    //        mgc_btree_tag_t tag
    //    ) override  {

    //        if ( id == "cond/timer/over_60s" ) {

    //            if ( duration.input_idle_time > 60'000 ) {
    //                return LeafResult::Success;
    //            } else {
    //                return LeafResult::Failure;
    //            }

    //        } else if ( id == "action/sleep" ) {

    //            anim_state_ = AnimState::Sleep;
    //            return LeafResult::Success;

    //        } else if ( id == "cond/timer/over_30s" ) {

    //            if ( duration.input_idle_time > 30'000 ) {
    //                return LeafResult::Success;
    //            } else {
    //                return LeafResult::Failure;
    //            }

    //        } else if ( id == "action/dance" ) {
    //            
    //            anim_state_ = AnimState::Dance;
    //            return LeafResult::Success;

    //        } else if ( id == "idle_breathing" ) {

    //            anim_state_ = AnimState::Normal;

    //            return LeafResult::Success;

    //        } else {

    //            return LeafResult::Running;
    //        }
    //    }

    //private:
    //    AnimState anim_state_ = AnimState::Normal;
    //} bt_listener_;
};


}// namespace devtest


#endif/*MGC_ENEMY_HPP*/

