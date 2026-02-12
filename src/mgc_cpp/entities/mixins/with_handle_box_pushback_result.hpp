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
