/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_GRAPHICS_PARALLAX_FACTOR_HPP
#define MGC_GRAPHICS_PARALLAX_FACTOR_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace graphics {

struct ParallaxFactor {
    float x = 0.0f;
    float y = 0.0f;
    
    constexpr ParallaxFactor() = default;
    constexpr ParallaxFactor(float x, float y) : x(x), y(y) {}

    explicit ParallaxFactor(const mgc_parallax_factor_t& data)
        : x(data.f_x), y(data.f_y) {}

    constexpr mgc_parallax_factor_t to_c() const {
        return mgc_parallax_factor_t{x, y};
    }
};


}// namespace graphics
}// namespace mgc

#endif/*MGC_GRAPHICS_PARALLAX_FACTOR_HPP*/
