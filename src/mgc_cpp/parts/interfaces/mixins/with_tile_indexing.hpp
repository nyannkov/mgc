/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_INDEXING_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_INDEXING_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithTileIndexing {

    void set_tile_index(size_t index) {
        static_cast<Derived*>(this)->set_tile_index_impl(index);
    }

    size_t get_tile_index() const {
        return static_cast<const Derived*>(this)->get_tile_index_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_TILE_INDEXING_HPP*/
