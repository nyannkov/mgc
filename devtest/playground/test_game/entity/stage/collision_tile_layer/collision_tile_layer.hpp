#ifndef MGC_COLLISION_TILE_LAYER_HPP
#define MGC_COLLISION_TILE_LAYER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "collision_tile_layer_id.hpp"
#include "collision_tile_index_hook.hpp"

namespace app {

struct CollisionTileLayer : mgc::entities::TilemapImpl<CollisionTileLayer> {
    
    explicit CollisionTileLayer(const FrameTimerT& frame_timer) 
        : current_layer_id_(CollisionTileLayerId::None),
          draw_hook_(frame_timer, current_layer_id_) {

        this->tilegrid().set_parallax_factor({1.0f, 1.0f});
        this->tilegrid().bind_listener(draw_hook_);
    }
    ~CollisionTileLayer() = default;

    const CollisionTileLayerId current_layer_id() const { return current_layer_id_; }
    void set_layer(CollisionTileLayerId id);

private:
    CollisionTileLayerId current_layer_id_;
    CollisionTileIndexHook draw_hook_;
};


} // namespace app

#endif/*MGC_COLLISION_TILE_LAYER_HPP*/
