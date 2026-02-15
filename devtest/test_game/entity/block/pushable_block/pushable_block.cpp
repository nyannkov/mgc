#include "entity/player/player.hpp"
#include "pushable_block.hpp"
#include "resources/generated/tileset/tileset_block.h"

namespace app {
namespace block {

PushableBlock::PushableBlock() {
    this->set_position({0, 0});
    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_block);
    this->mut_sprite().set_tile_index(0);

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({16, 16});
    hitboxes[0].set_enabled(false);
}

void PushableBlock::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    
    set_velocity({0, 0});
    this->set_player_push_direction(PlayerPushDirection::None);
}

void PushableBlock::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void PushableBlock::update_movement() {
    auto real_pos = this->precise_position();
    auto velocity = this->velocity();
    if ( velocity.y < (MGC_CELL_LEN-1) ) {
        velocity.y += 1.0f;
    }
    real_pos.y += velocity.y;
    this->set_velocity(velocity);
    this->set_precise_position(real_pos);
    this->set_player_push_direction(PlayerPushDirection::None);
}

void PushableBlock::resolve_movement() {
    auto real_pos = this->precise_position();
    switch ( player_push_direction() ) {
    case PlayerPushDirection::FromLeft:
        real_pos.x += MOVE_UNIT;
        break;
    case PlayerPushDirection::FromRight:
        real_pos.x -= MOVE_UNIT;
        break;
    default:
        break;
    }
    this->set_precise_position(real_pos);
}

bool PushableBlock::can_move_group(const ArrayViewer<Block*>& blocks) const {

   if ( player_push_direction() == PlayerPushDirection::None ) {
       return false;
   }

    for ( auto* block : blocks ) {

        if ( this == block ) {
            continue;
        }
        if ( block == nullptr ) {
            continue;
        }
        
        const auto result = ColBox2BoxT::query_pair(
            *this, static_cast<size_t>(BlockHitboxIndex::Body),
            *block, static_cast<size_t>(BlockHitboxIndex::Body),
            {
                {MOVE_UNIT, MOVE_UNIT, MOVE_UNIT, 0},
                {0, 0, 0, 0}
            }
        );

        if ( result.hit ) {
            if ( 0 < result.query_penetration.y && result.query_penetration.y < this->hitboxes()[0].size().height ) {
                return false;
            }
            if ( result.query_penetration.x < 0 ) {
                if ( player_push_direction() == PlayerPushDirection::FromLeft ) {
                    return false;
                }
            }
            if ( result.query_penetration.x > 0 ) {
                if ( player_push_direction() == PlayerPushDirection::FromRight ) {
                    return false;
                }
            }
        }
    }

    return true;
}

void PushableBlock::update_animation() {
}

void PushableBlock::on_player_hand_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    auto player_v = player.velocity();

    if ( player_v.x > 0 ) {
        this->set_player_push_direction(PlayerPushDirection::FromLeft);
    } else if ( player_v.x < 0 ) {
        this->set_player_push_direction(PlayerPushDirection::FromRight);
    } else {
        this->set_player_push_direction(PlayerPushDirection::None);
    }
}

void PushableBlock::on_block_hit(
    const Block& other,
    const mgc::collision::BoxCollisionInfo& info
) {

    auto velocity = this->velocity();
    if ( velocity.y != 0.0f ) {
        if ( info.signed_overlap.y > 0 ) {
            velocity.y = 0.0f;
        }
        this->set_velocity(velocity);
        auto pos = this->position();
        pos.y += info.signed_overlap.y;
        this->set_position(pos);
    }

}

void PushableBlock::on_collision_resolved(
    const stage::LayerBlock& block,
    const mgc::collision::MapPushbackInfo& info
) {
    auto velocity = this->velocity();
    if ( info.pushback.y < 0 ) {
        velocity.y = 0.0f;
    }
    this->set_velocity(velocity);
    auto pos = this->position();
    pos += info.pushback;
    this->set_position(pos);
}

void PushableBlock::on_collision_resolved(
    const mgc::collision::BoxPushbackInfo& info                               
) {
}

} // namespace app
} // namespace block

