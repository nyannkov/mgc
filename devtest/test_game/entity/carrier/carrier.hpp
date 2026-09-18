#ifndef MGC_CARRIER_HPP
#define MGC_CARRIER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/player/player_hitbox_index.hpp"

namespace app {
namespace carrier {

struct Carrier : mgc::entities::TilemapImpl<Carrier> {
    enum class OffBoardState {
        OffBoard,
        JustOffBoard,
        OnBoard
    };

    virtual ~Carrier() = default;
    virtual void spawn(const mgc::math::Vec2i& pos) = 0;
    virtual void despawn() = 0;
    virtual void update_movement() = 0;
    virtual void resolve_movement() = 0;
    virtual void update_animation() = 0;

    auto delta() const { return delta_; }
    OffBoardState off_board_state() const {
        switch ( off_board_counter_ ) {
        case 0:
            return OffBoardState::OffBoard;
        case MAX_OFF_BOARD_COUNTER_:
            return OffBoardState::OnBoard;
        default:
            return OffBoardState::JustOffBoard;
        }
    }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        using CleanedObj = std::decay_t<ObjT>;
        if constexpr (std::is_same_v<CleanedObj, Player>) {
            if ( info.obj_hitbox_index == 
                static_cast<size_t>(PlayerHitboxIndex::Body) 
            ) {
                on_collision_resolved(map, info);
            }
        }
    }

protected:
    void set_delta(mgc::math::Vec2f d) { delta_ = d; }
    void set_off_board_counter(int16_t counter) { 
        if ( counter < MAX_OFF_BOARD_COUNTER_ ) {
            off_board_counter_ = counter; 
        } else {
            off_board_counter_ = MAX_OFF_BOARD_COUNTER_;
        }
    }
    void update_off_board_counter() {
        if ( off_board_counter_ > 0 ) {
            off_board_counter_--;
        }
    }
    auto off_board_counter() const { return off_board_counter_; }

    virtual void on_collision_resolved(
        const carrier::Carrier& carrier,
        const mgc::collision::MapPushbackInfo& info
    ) { }

    static const int16_t MAX_OFF_BOARD_COUNTER_ = 2;

private:
    mgc::math::Vec2f delta_{0.0F, 0.0F};
    int16_t off_board_counter_ = 0;
};

} // namespace carrier
} // namespace app

#endif // MGC_CARRIER_HPP
