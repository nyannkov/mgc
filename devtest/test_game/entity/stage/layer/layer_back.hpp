#ifndef MGC_LAYER_BACK_HPP
#define MGC_LAYER_BACK_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/stage/layer/tile_draw_hook.hpp"

using mgc::collision::CollisionMap;
using mgc::parts::assets::TileIndexMap;
using mgc::parts::assets::Tileset;

namespace app {
namespace stage {

struct LayerBack : mgc::entities::TilemapImpl<LayerBack> {

    explicit LayerBack(const FrameTimerT& frame_timer)
        : tile_draw_hook_(frame_timer) {
        this->set_collision_enabled(false);
        this->mut_tilegrid().bind_listener(tile_draw_hook_);
    }
    ~LayerBack() = default;

    void set_maps(
        const TileIndexMap& tile_idx_map,
        const Tileset& tile_set
    ) {
        this->mut_tilegrid().set_tile_index_map(tile_idx_map);
        this->mut_tilegrid().set_tileset(tile_set);
    }

    void set_all_enabled(bool enable) {
        this->mut_tilegrid().set_visible(enable);
        tile_draw_hook_.set_tile_draw_hook_id(TileDrawHookId::None);
    }

    void set_parallax_factor(const mgc::graphics::ParallaxFactor &factor) {
        this->mut_tilegrid().set_parallax_factor(factor);
    }

    mgc::graphics::ParallaxFactor parallax_factor() const {
        return this->tilegrid().parallax_factor();
    }

    void set_tile_draw_hook_id(TileDrawHookId id) {
        tile_draw_hook_.set_tile_draw_hook_id(id);
    }

    TileDrawHookId tile_draw_hook_id() const {
        return tile_draw_hook_.tile_draw_hook_id();
    }

private:
    TileDrawHook tile_draw_hook_;
};

} // namespace stage
} // namespace app

#endif/*MGC_LAYER_BACK_HPP*/
