/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_ID_MAP_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_ID_MAP_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/assets/tile_id_map.hpp"


namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithTileIdMap {

    void set_tile_id_map(const mgc::parts::assets::TileIdMap& map) {
        static_cast<Derived*>(this)->set_tile_id_map_impl(map);
    }

    const mgc::parts::assets::TileIdMap * get_tile_id_map() const {
        return static_cast<const Derived*>(this)->get_tile_id_map_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_ID_MAP_HPP*/
