#include "entrance.hpp"
#include "game_context/game_context.hpp"
#include "resources/generated/tileset/tileset_static_objects_16x48.h"

namespace app {
namespace prop {

Entrance::Entrance() {
    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_static_objects_16x48);
    this->sprite().set_tile_index(0);
    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(16, 48);
    hitboxes[0].enabled = false;
}

void Entrance::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = true;
    this->set_position(pos);
    this->sprite().set_visible(true);
}

void Entrance::spawn(const mgc::math::Vec2i& pos, EntranceType type) {

    spawn(pos);

    type_ = type;

    switch (type_) {
    case EntranceType::Type1:
        this->sprite().set_tile_index(0);
        break;
    case EntranceType::Type2:
        this->sprite().set_tile_index(1);
        break;
    default:
        break;
    }
}

void Entrance::despawn() {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = false;
    this->sprite().set_visible(false);
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
