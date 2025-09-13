#ifndef MGC_COLLISION_TILE_INDEX_HOOK_HPP
#define MGC_COLLISION_TILE_INDEX_HOOK_HPP

#include "app_common.hpp"
#include "collision_tile_layer_id.hpp"

namespace app {

struct CollisionTileLayer;

struct CollisionTileIndexHook : mgc::parts::IBasicTilegridListener {

    CollisionTileIndexHook(
        const FrameTimerT& frame_timer,
        const CollisionTileLayerId& current_layer_id
    );

    uint8_t on_request_map_cell_value(
        uint8_t map_cell_value,
        uint16_t row,
        uint16_t col
    ) override;

private:
    StopwatchT sw_;
    const CollisionTileLayerId& current_layer_id_;
};

} // namespace app

#endif/*MGC_COLLISION_TILE_INDEX_HOOK_HPP*/

