/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_TILESET_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_TILESET_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/assets/tileset.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithTileset {

    void set_tileset(const assets::Tileset &tileset) {
        static_cast<Derived*>(this)->set_tileset_impl(tileset);
    }

    const assets::Tileset *get_tileset() const {
        return static_cast<const Derived*>(this)->get_tileset_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_TILESET_HPP*/
