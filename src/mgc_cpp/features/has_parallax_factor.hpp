/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_HAS_PARALLAX_FACTOR_HPP
#define MGC_FEATURES_HAS_PARALLAX_FACTOR_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/graphics/parallax_factor.hpp"

namespace mgc {
namespace features {

struct HasParallaxFactor {

    virtual ~HasParallaxFactor() = default;

    virtual void set_parallax_factor(const mgc::graphics::ParallaxFactor &factor) = 0;
    virtual mgc::graphics::ParallaxFactor parallax_factor() const;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_HAS_PARALLAX_FACTOR_HPP*/
