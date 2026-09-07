#include "entity/player/player.hpp"
#include "breakable_block.hpp"
#include "resources/generated/tileset/tileset_block.h"
#include "resources/generated/anim/block_breaking/anim_block_breaking.h"

namespace app {
namespace block {

BreakableBlock::BreakableBlock(
    const FrameTimerT& frame_timer
) : anim_(frame_timer) {
    this->set_position({0, 0});
    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_block);
    this->mut_sprite().set_tile_index(1);

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({16, 16});
    hitboxes[0].set_enabled(false);

    state_ = State::Inactive;
}

void BreakableBlock::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    set_velocity({0, 0});
    this->set_player_push_direction(PlayerPushDirection::None);

    state_ = State::Active;
}

void BreakableBlock::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);

    state_ = State::Inactive;
}

void BreakableBlock::update_movement() {
}

void BreakableBlock::resolve_movement() {
}

bool BreakableBlock::can_move_group(const ArrayViewer<Block*>& blocks) const {

   return false;
}

void BreakableBlock::update_animation() {
    
    if ( state_ == State::Despawning ) {
        if ( anim_.is_finished() ) {
            despawn();
        } else {
            anim_.proc();
            anim_.set_current_frame(this->mut_sprite());
        }
    }
}

void BreakableBlock::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( attack.can_break_block() ) {
        if ( state_ == State::Active ) {
            auto pos = this->position();
            pos += { -4, -4 };
            this->set_position(pos);

            auto& hitboxes = this->mut_hitboxes();
            hitboxes[0].set_enabled(false);

            anim_.set_anim_frames(anim_block_breaking_block_breaking);
            anim_.set_loop(false);
            anim_.set_current_frame(this->mut_sprite());
            anim_.start_animation();

            state_ = State::Despawning;
        }
    }
}


} // namespace app
} // namespace block

