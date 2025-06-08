/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_ITILEMAP_HPP
#define MGC_PARTS_INTERFACES_ITILEMAP_HPP

#include "mixins/with_tileset.hpp"

namespace mgc {
namespace parts {
namespace interfaces {

template <typename Derived>
struct ITilemap : mgc::parts::interfaces::mixins::WithTileset<Derived> { };

}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_ITILEMAP_HPP*/
