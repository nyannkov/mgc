/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_IDIALOGUEBOX_HPP
#define MGC_PARTS_INTERFACES_IDIALOGUEBOX_HPP

#include "mixins/with_size.hpp"
#include "mixins/with_margin.hpp"
#include "mixins/with_font.hpp"
#include "mixins/with_text.hpp"
#include "mixins/with_auto_text_scroll.hpp"
#include "mixins/with_typing_effect.hpp"

namespace mgc {
namespace parts {
namespace interfaces {

template <typename Derived>
struct IDialoguebox : mgc::parts::interfaces::mixins::WithSize<Derived>,
                      mgc::parts::interfaces::mixins::WithMargin<Derived>,
                      mgc::parts::interfaces::mixins::WithFont<Derived>,
                      mgc::parts::interfaces::mixins::WithText<Derived>,
                      mgc::parts::interfaces::mixins::WithAutoTextScroll<Derived>,
                      mgc::parts::interfaces::mixins::WithTypingEffect<Derived> { };

}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_IDIALOGUEBOX_HPP*/
