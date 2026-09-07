#ifndef MGC_BREAKABLE_BLOCK_HPP
#define MGC_BREAKABLE_BLOCK_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/block/block.hpp"

namespace app {
namespace block {

struct BreakableBlock : Block {
    enum class State {
        Inactive,
        Active,
        Despawning
    };

    explicit BreakableBlock(
        const FrameTimerT& frame_timer
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void despawn() override;
    void update_movement() override;
    void resolve_movement() override;
    void update_animation() override;
    bool can_move_group(const ArrayViewer<Block*>& blocks) const override;

private:
    AnimControllerT anim_;
    State state_;
    void on_attack_hit(
        const attack::Attack& attack,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

};

} // namespace block
} // namespace app

#endif // MGC_BREAKABLE_BLOCK_HPP
