/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_RESETTABLE_HPP
#define MGC_FEATURES_RESETTABLE_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace features {

struct Resettable {

    virtual ~Resettable() = default;

    virtual void reset() = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_RESETTABLE_HPP*/
