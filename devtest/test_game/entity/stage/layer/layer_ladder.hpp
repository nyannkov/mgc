#ifndef MGC_LAYER_LADDER_HPP
#define MGC_LAYER_LADDER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"

using mgc::collision::CollisionMap;
using mgc::parts::assets::TileIndexMap;
using mgc::parts::assets::Tileset;

namespace app {
namespace stage {

struct LayerLadder : mgc::entities::TilemapImpl<LayerLadder> {

    ~LayerLadder() = default;

    void set_maps(
        const TileIndexMap& tile_idx_map,
        const Tileset& tile_set,
        const CollisionMap *col_map
    ) {
        this->mut_tilegrid().set_tile_index_map(tile_idx_map);
        this->mut_tilegrid().set_tileset(tile_set);
        this->set_collision_map(col_map);
    }

    void set_all_enabled(bool enable) {
        this->mut_tilegrid().set_visible(enable);
        this->set_collision_enabled(enable);
    }
};

} // namespace stage
} // namespace app

#endif/*MGC_LAYER_LADDER_HPP*/
