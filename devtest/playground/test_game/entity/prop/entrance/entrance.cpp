#include "entrance.hpp"
#include "game_context/game_context.hpp"
#include "resources/generated/tileset/tileset_static_objects.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Entrance::Entrance(StageInfo& stage_info, const GamepadT& gamepad) 
    : stage_info_(stage_info),
      gamepad_(gamepad) {

    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_static_objects);
    this->sprite().set_tile_index(3);
    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(16, 16);
    hitboxes[0].enabled = false;
}

void Entrance::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = true;
    this->set_position(pos);
    this->sprite().set_visible(true);
}

void Entrance::spawn(const mgc::math::Vec2i& pos, EntranceDirection dir, GameContext& ctx) {
    
    spawn(pos);
    direction_ = dir;
    if ( dir == EntranceDirection::Down ) {
        if ( ctx.stage_info().current_floor() == 0 ) {
            this->sprite().set_tile_index(4);
        } else {
            this->sprite().set_tile_index(2);
        }
    } else {
        this->sprite().set_tile_index(3);
    }
}

void Entrance::despawn() {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = false;
    this->sprite().set_visible(false);
}

void Entrance::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Up) ) {
        auto current_floor = stage_info_.current_floor();
        if ( direction_ == EntranceDirection::Up ) {
            stage_info_.set_current_floor(current_floor + 1);
            stage_info_.set_next_scene_request(next_scene_);
        } else {
            if ( stage_info_.current_floor() > 0 ) {
                stage_info_.set_current_floor(current_floor - 1);
                stage_info_.set_next_scene_request(next_scene_);
            } else {
                // Nothing to do.
            }
        }
    }
}

} // namespace prop
} // namespace app
