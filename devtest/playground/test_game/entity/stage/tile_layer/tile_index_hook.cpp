#include "tile_index_hook.hpp"
#include "tile_layer.hpp"

namespace app {

TileIndexHook::TileIndexHook(
    const FrameTimerT& frame_timer,
    const TileLayerId& current_layer_id
) : sw_(frame_timer),
    current_layer_id_(current_layer_id) {

    sw_.reset();
    sw_.start();
}

uint8_t TileIndexHook::on_request_map_cell_value(
    uint8_t map_cell_value,
    uint16_t row,
    uint16_t col
) {
    uint8_t tileset_index = MGC_GET_MAP_TILESET_INDEX(map_cell_value);
    bool hit_flag = MGC_GET_MAP_HIT_FLAG(map_cell_value);
    uint32_t count_100ms = sw_.elapsed_ms()/100;

    switch (current_layer_id_) {
    case TileLayerId::Layer_001:
        if ( tileset_index == 20 ) {
            // Flame flickering effect for tileset_index 20 ( tile index is 21 )
            switch(count_100ms%3) {
            case 1: tileset_index += 1; break;
            case 2: tileset_index += 2; break;
            default: break;
            }
        }
        break;

    default:
        break;
    }

    return MGC_MAP_CELL_VALUE(hit_flag, tileset_index);
}

} // namespace app
