#include "mgc_cpp/mgc.hpp"
#include "tile_layer.hpp"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/map/map_bg_layer_01.h"

namespace app {

void TileLayer::set_layer(TileLayerId id) {
    current_layer_id_ = id;
    switch (id) {
    case TileLayerId::Layer_001:
        this->tilegrid().set_tileset(tileset_map_elements);
        this->tilegrid().set_tile_index_map(map_bg_layer_01);
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
