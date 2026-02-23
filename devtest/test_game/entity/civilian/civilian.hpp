#ifndef MGC_CIVILIAN_HPP
#define MGC_CIVILIAN_HPP

#include "mgc_cpp/mgc.hpp"
#include "entity/stage/layer/layer_block.hpp"
#include "app_common.hpp"
#include "scene/request/talkflow_request.hpp"
#include "entity/player/player_hitbox_index.hpp"

namespace app {
namespace civilian {

enum class CivilHitboxIndex : size_t {
    Body = 0,
    View,
    Count
};

struct Civilian : 
    mgc::entities::ActorImpl<
        Civilian, static_cast<size_t>(CivilHitboxIndex::Count)
    > {

    virtual ~Civilian() = default;

    virtual void spawn(const mgc::math::Vec2i& pos) = 0;
    virtual void despawn() = 0;
    virtual void update_movement() = 0;
    virtual void update_animation() = 0;

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, Player>) {
            switch( info.self_hitbox_index) {
            case static_cast<size_t>(CivilHitboxIndex::Body):
                if ( info.other_hitbox_index == 
                    static_cast<size_t>(PlayerHitboxIndex::Body) 
                ) {
                    on_player_hit(other, info);
                }
                break;
            case static_cast<size_t>(CivilHitboxIndex::View):
                on_player_in_view(other, info);
                break;
            default:
                break;
            }
        } else { }
    }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        if constexpr (std::is_same_v<MapT, stage::LayerBlock>) {
            
            on_collision_resolved(map, info);
        }
    }

    const auto* take_talkflow_request() {
        const TalkflowRequest* r = talkflow_req_;
        talkflow_req_ = nullptr;
        return r;
    }

    void set_talkflow_request(TalkflowRequest request) {
        talkflow_req_buf_ = request;
        talkflow_req_set_flag_ = true;
    }

protected:
    virtual void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_player_in_view(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) { }

    void trigger_talkflow_request() {
        if ( talkflow_req_set_flag_ ) {
            talkflow_req_ = &talkflow_req_buf_;
        }
    }

    void set_and_trigger_talkflow_request(TalkflowRequest request) {
        set_talkflow_request(request);
        trigger_talkflow_request();
    }

    void flush_talkflow_request() {
        talkflow_req_ = nullptr;
        talkflow_req_set_flag_ = false;
    }

private:
    TalkflowRequest talkflow_req_buf_ {};
    TalkflowRequest* talkflow_req_ = nullptr;
    bool talkflow_req_set_flag_ = false;
};

}// namespace civilian 
}// namespace app

#endif/*MGC_CIVILIAN_HPP*/

