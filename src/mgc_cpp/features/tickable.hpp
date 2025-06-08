/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_TICKABLE_HPP
#define MGC_FEATURES_TICKABLE_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace features {

struct Tickable {

    virtual ~Tickable() = default;
    virtual void on_tick(uint32_t delta_ms) = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_TICKABLE_HPP*/
