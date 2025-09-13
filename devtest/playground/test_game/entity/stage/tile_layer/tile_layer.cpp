#include "mgc_cpp/mgc.hpp"
#include "tile_layer.hpp"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/map/map_scene_001_bg1.h"
#include "resources/generated/map/map_scene_001_bg2.h"

namespace app {

void TileLayer::set_layer(TileLayerId id) {
    current_layer_id_ = id;
    switch (id) {
    case TileLayerId::Layer_001_Background_1:
        this->tilegrid().set_tileset(tileset_map_elements);
        this->tilegrid().set_tile_index_map(map_scene_001_bg1);
        this->tilegrid().set_visible(true);
        break;

    case TileLayerId::Layer_001_Background_2:
        this->tilegrid().set_tileset(tileset_map_elements);
        this->tilegrid().set_tile_index_map(map_scene_001_bg2);
        this->tilegrid().set_visible(true);
        break;

    case TileLayerId::None:/*fallthrough*/
    default:
        this->tilegrid().set_visible(false);
        break;
    }
}

void TileLayer::set_parallax_factor(const mgc::graphics::ParallaxFactor &factor) {
    this->tilegrid().set_parallax_factor(factor);
}

} // namespace app
