#include "collision_tile_index_hook.hpp"
#include "collision_tile_layer.hpp"

namespace app {

CollisionTileIndexHook::CollisionTileIndexHook(
    const FrameTimerT& frame_timer,
    const CollisionTileLayerId& current_layer_id
) : sw_(frame_timer),
    current_layer_id_(current_layer_id) {

    sw_.reset();
    sw_.start();
}

uint8_t CollisionTileIndexHook::on_request_map_cell_value(
    uint8_t map_cell_value,
    uint16_t row,
    uint16_t col
) {

    uint8_t tileset_index = MGC_GET_MAP_TILESET_INDEX(map_cell_value);
    bool hit_flag = MGC_GET_MAP_HIT_FLAG(map_cell_value);

    switch (current_layer_id_) {
    case CollisionTileLayerId::Layer_001:
        break;

    case CollisionTileLayerId::None:/*fallthrough*/
    default:
        break;
    }

    return MGC_MAP_CELL_VALUE(hit_flag, tileset_index);
}

} // namespace app
