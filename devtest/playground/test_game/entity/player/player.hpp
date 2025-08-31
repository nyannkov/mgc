#ifndef MGC_PLAYER_HPP
#define MGC_PLAYER_HPP

#include "app_common.hpp"
#include "entity/stage/collision_tile_layer/collision_tile_layer.hpp"
#include "player_anim.hpp"

namespace app {

struct Player : mgc::entities::ActorImpl<Player, 1> {
    Player(const FrameTimerT& frame_timer, const GamepadT& gamepad);
    ~Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    void set_spawn_point(const mgc::math::Vec2i& pos, PlayerAnimState anim_state);
    void prepare_update();
    void finalize_update();

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        if constexpr (std::is_same_v<MapT, CollisionTileLayer>) {

            auto pos = this->position();
            // Pushback response logic: stop falling or bounce depending on direction
            if ( info.pushback.y < 0 ) {
                velocity_.y = 0.0f;
                is_grounded_ = true;
            } else if ( info.pushback.y > 0 ) {
                velocity_.y *= -1;
            } else { 
            }
            pos += info.pushback;

            this->set_position(pos);
        }
    }

private:
    const GamepadT& gamepad_;
    const FrameTimerT& frame_timer_;
    mgc::control::anim::AnimController<FrameTimerT> anim_;
    mgc::math::Vec2f velocity_;
    bool is_grounded_;
    bool is_grounded_pre_;
    PlayerAnimState anim_state_;
    bool is_right_;
};


}// namespace app


#endif/*MGC_PLAYER_HPP*/

