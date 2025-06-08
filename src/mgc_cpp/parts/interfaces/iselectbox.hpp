/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_ISELECTBOX_HPP
#define MGC_PARTS_INTERFACES_ISELECTBOX_HPP

#include "mixins/with_size.hpp"
#include "mixins/with_padding.hpp"
#include "mixins/with_font.hpp"
#include "mixins/with_items.hpp"

namespace mgc {
namespace parts {
namespace interfaces {

template <typename Derived, typename ItemT>
struct ISelectbox : mgc::parts::interfaces::mixins::WithSize<Derived>,
                    mgc::parts::interfaces::mixins::WithPadding<Derived>,
                    mgc::parts::interfaces::mixins::WithFont<Derived>,
                    mgc::parts::interfaces::mixins::WithItems<Derived, ItemT> { };

}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_ISELECTBOX_HPP*/
