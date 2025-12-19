#ifndef MGC_TILE_DRAW_HOOK_HPP
#define MGC_TILE_DRAW_HOOK_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"

namespace app {
namespace stage {
    
enum class TileDrawHookId {
    None,
    TowerFront_LayerBack0,
};

struct TileDrawHook : mgc::parts::IBasicTilegridListener {
    explicit TileDrawHook(const FrameTimerT& frame_timer)
        : sw_(frame_timer) {
        sw_.reset();
        sw_.start();
    }

    uint8_t on_request_map_cell_value(
        uint8_t map_cell_value,
        uint16_t row,
        uint16_t col
    ) override;

    void set_tile_draw_hook_id(TileDrawHookId id) { hook_id_ = id; }
    TileDrawHookId tile_draw_hook_id() const { return hook_id_; }

private:
    StopwatchT sw_;
    TileDrawHookId hook_id_ = TileDrawHookId::None;
};

} // namespace stage
} // namespace app

#endif // MGC_TILE_DRAW_HOOK_HPP
