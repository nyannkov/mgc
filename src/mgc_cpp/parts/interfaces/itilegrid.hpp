/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_ITILEGRID_HPP
#define MGC_PARTS_INTERFACES_ITILEGRID_HPP

#include "mixins/with_tileset.hpp"
#include "mixins/with_tile_id_map.hpp"

namespace mgc {
namespace parts {
namespace interfaces {

template <typename Derived>
struct ITilegrid : mgc::parts::interfaces::mixins::WithTileset<Derived>,
                   mgc::parts::interfaces::mixins::WithTileIdMap<Derived> { };

}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_ITILEGRID_HPP*/
