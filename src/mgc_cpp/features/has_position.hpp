/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_HAS_POSITION_HPP
#define MGC_FEATURES_HAS_POSITION_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace features {

template <typename VecT>
struct HasPosition {

    virtual ~HasPosition() = default;

    virtual VecT position() const = 0;
    virtual void set_position(const VecT& position) = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_HAS_POSITION_HPP*/
