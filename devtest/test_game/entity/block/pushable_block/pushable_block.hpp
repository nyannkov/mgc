#ifndef MGC_PUSHABLE_BLOCK_HPP
#define MGC_PUSHABLE_BLOCK_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/block/block.hpp"

namespace app {
namespace block {

struct PushableBlock : Block {

    PushableBlock();

    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    void update_movement() override;
    void resolve_movement() override;
    void update_animation() override;
    bool can_move_group(const ArrayViewer<Block*>& blocks) const override;

private:
    static constexpr int32_t MOVE_UNIT = 1;

    void on_player_hand_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_block_hit(
        const Block& other,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    void on_collision_resolved(
        const stage::LayerBlock& block,
        const mgc::collision::MapPushbackInfo& info
    ) override;

    void on_collision_resolved(
        const mgc::collision::BoxPushbackInfo& info                               
    ) override;
};

} // namespace block
} // namespace app

#endif // MGC_PUSHABLE_BLOCK_HPP
