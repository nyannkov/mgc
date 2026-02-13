/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_ENTITIES_MIXINS_WITH_HANDLE_BOX_PUSHBACK_RESULT_HPP
#define MGC_ENTITIES_MIXINS_WITH_HANDLE_BOX_PUSHBACK_RESULT_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/box_pushback_info.hpp"

namespace mgc {
namespace entities {
namespace mixins {

template <typename, typename = void>
struct has_handle_box_pushback_result : std::false_type {};

template <typename T>
struct has_handle_box_pushback_result<
    T,
    std::void_t<
        decltype(std::declval<const T&>().hitboxes())
    >
> : std::true_type {};

template <typename Derived>
struct WithHandleBoxPushbackResult {
    template <typename View>
    void handle_box_pushback_result(
            const mgc::collision::BoxPushbackInfo& info,
            const View& others
    ) { 
        static_cast<Derived*>(this)->handle_box_pushback_result_impl(info, others);
    }
};


}// namespace mixins
}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_MIXINS_WITH_HANDLE_MAP_PUSHBACK_RESULT_HPP*/
