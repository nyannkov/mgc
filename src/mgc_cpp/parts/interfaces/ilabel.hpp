/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_ILABEL_HPP
#define MGC_PARTS_INTERFACES_ILABEL_HPP

#include "mixins/with_size.hpp"
#include "mixins/with_text.hpp"
#include "mixins/with_font.hpp"

namespace mgc {
namespace parts {
namespace interfaces {

template <typename Derived>
struct ILabel : mgc::parts::interfaces::mixins::WithSize<Derived>,
                mgc::parts::interfaces::mixins::WithText<Derived>,
                mgc::parts::interfaces::mixins::WithFont<Derived> { };

}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_ILABEL_HPP*/
