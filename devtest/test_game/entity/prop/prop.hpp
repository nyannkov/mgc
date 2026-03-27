#ifndef MGC_PROP_HPP
#define MGC_PROP_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/request/talkflow_request.hpp"
#include "scene/request/scene_transition_request.hpp"
#include "entity/player/player_hitbox_index.hpp"

namespace app {
namespace prop {

enum class PropHitboxId : size_t {
    Body = 0,
    Count
};

struct Prop : mgc::entities::ActorImpl<
        Prop, static_cast<size_t>(PropHitboxId::Count)
    > {
    
    virtual ~Prop() = default;

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
            if ( info.other_hitbox_index == 
                static_cast<size_t>(PlayerHitboxIndex::Body) 
            ) {
                on_player_hit(other, info);
            }
        } else if constexpr (std::is_same_v<Other, attack::Attack>) {
            on_attack_hit(other, info);

        } else { }
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

    const auto* take_scene_transition_request() {
        const SceneTransitionRequest* r = scene_trans_req_;
        scene_trans_req_ = nullptr;
        return r; 
    }

    void set_scene_transition_request(SceneTransitionRequest request) {
        scene_trans_req_buf_ = request;
        scene_trans_req_set_flag_ = true;
    }

protected:
    virtual void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
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

    void trigger_scene_transition_request() {
        if ( scene_trans_req_set_flag_ ) {
            scene_trans_req_ = &scene_trans_req_buf_;
        }
    }

    void flush_scene_transition_request() {
        scene_trans_req_ = nullptr;
        scene_trans_req_set_flag_ = false;
    }

    void set_and_trigger_scene_transition_request(SceneTransitionRequest request) {
        set_scene_transition_request(request);
        trigger_scene_transition_request();
    }

private:
    TalkflowRequest talkflow_req_buf_ {};
    TalkflowRequest* talkflow_req_ = nullptr;
    bool talkflow_req_set_flag_ = false;

    SceneTransitionRequest scene_trans_req_buf_ {};
    SceneTransitionRequest* scene_trans_req_ = nullptr;
    bool scene_trans_req_set_flag_ = false;
};

} // namespace prop
} // namespace app

#endif // MGC_PROP_HPP
