#ifndef MGC_TILE_LAYER_HPP
#define MGC_TILE_LAYER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "tile_layer_id.hpp"
#include "tile_index_hook.hpp"

namespace app {

struct TileLayer : mgc::entities::TilemapImpl<TileLayer> {
    
    explicit TileLayer(const FrameTimerT& frame_timer) 
        : current_layer_id_(TileLayerId::None),
          draw_hook_(frame_timer, current_layer_id_) {

        this->set_collision_enabled(false);
        this->tilegrid().bind_listener(draw_hook_);
    }
    ~TileLayer() = default;

    const TileLayerId current_layer_id() const { return current_layer_id_; }
    void set_layer(TileLayerId id);
    void set_parallax_factor(const mgc::graphics::ParallaxFactor &factor);

private:
    TileLayerId current_layer_id_;
    TileIndexHook draw_hook_;
};


} // namespace app

#endif/*MGC_TILE_LAYER_HPP*/
