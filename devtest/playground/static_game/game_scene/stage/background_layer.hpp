#ifndef MGC_BACKGROUND_LAYER_HPP
#define MGC_BACKGROUND_LAYER_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/map/map_bg_layer_01.h"

namespace app {

struct BackgroundLayer : mgc::entities::TilemapImpl<BackgroundLayer> { 

    enum class LayerId : mgc_id_t {
        None = 0,
        Layer1
    };

    BackgroundLayer() : layer_id_(LayerId::None), tilegrid_listener_(layer_id_) {
        this->tilegrid().reset();
        this->tilegrid().bind_listener(tilegrid_listener_);
    };

    void update() {
        tilegrid_listener_.update();
    }

    void setup_by_id(LayerId layer_id) {

        layer_id_ = layer_id;

        mgc_id_t id = static_cast<mgc_id_t>(layer_id);

        switch (layer_id) {
        case LayerId::Layer1:
            this->tilegrid().set_id(id);
            this->tilegrid().set_tileset(tileset_map_elements);
            this->tilegrid().set_tile_index_map(map_bg_layer_01);
            this->tilegrid().set_parallax_factor(mgc::graphics::ParallaxFactor(0.5f, 1.0f));
            this->set_collision_enabled(false);
            break;

        case LayerId::None:/*fallthrough*/
        default:
            this->tilegrid().set_id(id);
            this->set_collision_enabled(false);
            break;
        }
    }

    const LayerId layer_id() const { return layer_id_; }

private:
    LayerId layer_id_;
    struct TilegridListener : mgc::parts::IBasicTilegridListener {

        TilegridListener(const LayerId& layer_id)
            : counter_(0), layer_id_(layer_id) { }

        void update() { counter_++; }

        uint8_t on_request_map_cell_value(uint8_t map_cell_value, uint16_t row, uint16_t col) override {
            uint8_t tileset_index = MGC_GET_MAP_TILESET_INDEX(map_cell_value);
            bool hit_flag = MGC_GET_MAP_HIT_FLAG(map_cell_value);
            if ( layer_id_ == LayerId::Layer1 ) {
                if ( tileset_index == 20 ) {
                    // Flame flickering effect for tileset_index 20 (used in tilemap listener)
                    tileset_index += counter_%3;
                }
            }
            return MGC_MAP_CELL_VALUE(hit_flag, tileset_index);
        }

    private:
        uint32_t counter_;
        const LayerId& layer_id_;
    } tilegrid_listener_;
};

} // namespace app

#endif/*MGC_BACKGROUND_LAYER_HPP*/

