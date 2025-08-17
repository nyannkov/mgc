/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_INDEX_MAP_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_INDEX_MAP_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/assets/tile_index_map.hpp"


namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithTileIndexMap {

    void set_tile_index_map(const mgc::parts::assets::TileIndexMap& map) {
        static_cast<Derived*>(this)->set_tile_index_map_impl(map);
    }

    const mgc::parts::assets::TileIndexMap * tile_index_map() const {
        return static_cast<const Derived*>(this)->tile_index_map_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_INDEX_MAP_HPP*/
