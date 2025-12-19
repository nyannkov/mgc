#include "tile_draw_hook.hpp"

namespace app {
namespace stage {

uint8_t TileDrawHook::on_request_map_cell_value(
    uint8_t map_cell_value,
    uint16_t row,
    uint16_t col
) {
    uint8_t tileset_index = MGC_GET_MAP_TILESET_INDEX(map_cell_value);
    bool hit_flag = MGC_GET_MAP_HIT_FLAG(map_cell_value);
    uint32_t count_100ms = sw_.elapsed_ms()/100;
    
    switch (hook_id_) {
    case TileDrawHookId::TowerFront_LayerBack0: 
    {
        // Waterfall @ tilemap_map_element
        if ( count_100ms%2 == 0 ) {
            if ( tileset_index == 42 ) {
                tileset_index = 43;
            } else if ( tileset_index == 43 ) {
                tileset_index = 42;
            } else {
            }
        }
        break;
    }
    case TileDrawHookId::None:/*FALLTHROUGH*/
    default:
        break;
    }

    return MGC_MAP_CELL_VALUE(hit_flag, tileset_index);
}

} // namespace stage
} // namespace app

