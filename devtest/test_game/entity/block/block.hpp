#ifndef MGC_BLOCK_HPP
#define MGC_BLOCK_HPP

#include "mgc_cpp/mgc.hpp"
#include "entity/stage/layer/layer_block.hpp"
#include "app_common.hpp"
#include "entity/player/player_hitbox_index.hpp"

namespace app {
namespace block {

enum class BlockHitboxIndex : size_t {
    Body = 0,
    Count
};

struct Block : mgc::entities::ActorImpl<
        Block, static_cast<size_t>(BlockHitboxIndex::Count)
    > {

    enum class PlayerPushDirection {
        None,
        FromLeft,
        FromRight
    };

    virtual ~Block() = default;

    virtual void spawn(const mgc::math::Vec2i& pos) = 0;
    virtual void despawn() = 0;
    virtual void update_movement() = 0;
    virtual void resolve_movement() = 0;
    virtual void update_animation() = 0;
    virtual bool can_move_group(const ArrayViewer<Block*>& blocks) const = 0;

    PlayerPushDirection player_push_direction() const { return player_push_direction_; }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, Player>) {
            if ( info.other_hitbox_index == 
                static_cast<size_t>(PlayerHitboxIndex::Hand) 
            ) {
                on_player_hand_hit(other, info);
            }
        } else if constexpr (std::is_same_v<Other, Block>) {
            on_block_hit(other, info);
        }
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

    template <typename View>
    void handle_box_pushback_result_impl(                                              
            const mgc::collision::BoxPushbackInfo& info,                               
            const View& others
    ) { 
        if constexpr (std::is_same_v<View, mgc::collision::BoxBoxArrayView<block::Block>>) {
            on_collision_resolved(info);
        }
    }   



    mgc::math::Vec2f velocity() const { return velocity_; }

protected:
    virtual void on_player_hand_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_block_hit(
        const Block& other,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    virtual void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) { }

    virtual void on_collision_resolved(
        const mgc::collision::BoxPushbackInfo& info                               
    ) { }

    void set_player_push_direction(PlayerPushDirection dir) { player_push_direction_ = dir; }

    void set_velocity(mgc::math::Vec2f v) { velocity_ = v; }

private:
    PlayerPushDirection player_push_direction_ = PlayerPushDirection::None;
    mgc::math::Vec2f velocity_{};
};

} // namespace block
} // namespace app

#endif // MGC_BLOCK_HPP
