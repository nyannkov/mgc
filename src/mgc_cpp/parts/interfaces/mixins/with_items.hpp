/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_ITEMS_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_ITEMS_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived, typename ItemT>
struct WithItems {

    void add_item(ItemT item) {
        static_cast<Derived*>(this)->add_item_impl(item);
    }

    void remove_item_by_index(int32_t index) {
        static_cast<Derived*>(this)->remove_item_by_index_impl(index);
    }

    void clear_items() {
        static_cast<Derived*>(this)->clear_items_impl();
    }

    size_t get_item_count() const {
        return static_cast<const Derived*>(this)->get_item_count_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_ITEMS_HPP*/
