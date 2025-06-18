/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_ITEM_SELECTABLE_HPP
#define MGC_FEATURES_ITEM_SELECTABLE_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace features {

struct ItemSelectable {

    virtual ~ItemSelectable() = default;

    virtual void set_selected_index(int32_t index) = 0;
    virtual int32_t selected_index() const = 0;
    virtual void select_next() = 0;
    virtual void select_previous() = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_ITEM_SELECTABLE_HPP*/
