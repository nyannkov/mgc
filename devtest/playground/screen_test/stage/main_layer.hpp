#ifndef MGC_MAIN_LAYER_HPP
#define MGC_MAIN_LAYER_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/map/map_main_layer_01.h"

namespace app {

struct MainLayer : mgc::entities::TilemapImpl<MainLayer> { 

    enum class LayerId : mgc_id_t {
        None = 0,
        Layer1
    };

    MainLayer() : layer_id_(LayerId::None) { };

    void setup_by_id(LayerId layer_id) {

        layer_id_ = layer_id;

        mgc_id_t id = static_cast<mgc_id_t>(layer_id);

        switch (layer_id) {
        case LayerId::Layer1:
            this->tilegrid().set_id(id);
            this->tilegrid().set_tileset(tileset_map_elements);
            this->tilegrid().set_tile_id_map(map_main_layer_01);
            this->set_collision_map(&map_main_layer_01);
            this->set_collision_enabled(true);
            break;

        case LayerId::None:/*fallthrough*/
        default:
            this->tilegrid().reset();
            this->tilegrid().set_id(id);
            this->set_collision_enabled(false);
            break;
        }
    }

    const LayerId layer_id() const { return layer_id_; }

private:
    LayerId layer_id_;
};

} // namespace app

#endif/*MGC_MAIN_LAYER_HPP*/

