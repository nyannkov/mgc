/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ENTITIES_MIXINS_WITH_HANDLE_MAP_PUSHBACK_RESULT_HPP
#define MGC_ENTITIES_MIXINS_WITH_HANDLE_MAP_PUSHBACK_RESULT_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/collision/map_pushback_info.hpp"

namespace mgc {
namespace entities {
namespace mixins {

template <typename Derived>
struct WithHandleMapPushbackResult {
    template <typename ObjT, typename MapT>
    void handle_map_pushback_result(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) {
        static_cast<Derived*>(this)->handle_map_pushback_result_impl(obj, map, info);
    }
};


}// namespace mixins
}// namespace entities
}// namespace mgc

#endif/*MGC_ENTITIES_MIXINS_WITH_HANDLE_MAP_PUSHBACK_RESULT_HPP*/
