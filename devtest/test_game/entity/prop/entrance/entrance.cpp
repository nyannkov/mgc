#include "entrance.hpp"
#include "game_context/game_context.hpp"
#include "resources/generated/tileset/tileset_static_objects_16x48.h"

namespace app {
namespace prop {

Entrance::Entrance() {
    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_static_objects_16x48);
    this->mut_sprite().set_tile_index(0);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({16, 48});
    hitboxes[0].set_enabled(false);
}

void Entrance::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
}

void Entrance::spawn(const mgc::math::Vec2i& pos, EntranceType type) {

    spawn(pos);

    type_ = type;

    switch (type_) {
    case EntranceType::Type1:
        this->mut_sprite().set_tile_index(0);
        break;
    case EntranceType::Type2:
        this->mut_sprite().set_tile_index(1);
        break;
    default:
        break;
    }
}

void Entrance::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    flush_talkflow_request();
    flush_scene_transition_request();
}

void Entrance::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    trigger_scene_transition_request();
}

} // namespace prop
} // namespace app
