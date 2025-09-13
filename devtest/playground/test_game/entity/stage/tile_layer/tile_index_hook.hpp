#ifndef MGC_TILE_INDEX_HOOK_HPP
#define MGC_TILE_INDEX_HOOK_HPP

#include "app_common.hpp"
#include "tile_layer_id.hpp"

namespace app {

struct TileLayer;

struct TileIndexHook : mgc::parts::IBasicTilegridListener {

    TileIndexHook(
        const FrameTimerT& frame_timer,
        const TileLayerId& current_layer_id
    );

    uint8_t on_request_map_cell_value(
        uint8_t map_cell_value,
        uint16_t row,
        uint16_t col
    ) override;

private:
    StopwatchT sw_;
    const TileLayerId& current_layer_id_;
};

} // namespace app

#endif/*MGC_TILE_INDEX_HOOK_HPP*/
