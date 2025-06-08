/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_ISPRITE_HPP
#define MGC_PARTS_INTERFACES_ISPRITE_HPP

#include "mixins/with_tileset.hpp"
#include "mixins/with_tile_indexing.hpp"
#include "mixins/with_trim_region.hpp"

namespace mgc {
namespace parts {
namespace interfaces {

template <typename Derived>
struct ISprite : mgc::parts::interfaces::mixins::WithTileset<Derived>,
                 mgc::parts::interfaces::mixins::WithTileIndexing<Derived>,
                 mgc::parts::interfaces::mixins::WithTrimRegion<Derived> { };

}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_ISPRITE_HPP*/
