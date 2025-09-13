#include "mgc_cpp/mgc.hpp"
#include "collision_tile_layer.hpp"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/map/map_scene_001_main.h"

namespace app {

void CollisionTileLayer::set_layer(CollisionTileLayerId id) {
    current_layer_id_ = id;
    switch (id) {
    case CollisionTileLayerId::Layer_001:
        this->tilegrid().set_tileset(tileset_map_elements);
        this->tilegrid().set_tile_index_map(map_scene_001_main);
        this->tilegrid().set_visible(true);
        this->set_collision_map(&map_scene_001_main);
        this->set_collision_enabled(true);
        break;

    case CollisionTileLayerId::None:/*fallthrough*/
    default:
        this->tilegrid().set_visible(false);
        this->set_collision_enabled(false);
        break;
    }
}

} // namespace app
