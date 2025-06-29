/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_TYPES_PADDING_HPP
#define MGC_PARTS_TYPES_PADDING_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace types {

struct Padding {
    uint8_t top = 0;
    uint8_t bottom = 0;
    uint8_t left = 0;
    uint8_t right = 0;

    constexpr Padding() = default;
    constexpr Padding(uint8_t top, uint8_t bottom, uint8_t left, uint8_t right) 
        : top(top), bottom(bottom), left(left), right(right) {}

    explicit Padding(const mgc_padding_t& data)
        : top(data.top), bottom(data.bottom), left(data.left), right(data.right) {} 

    constexpr mgc_padding_t to_c() const {
        return mgc_padding_t{left, right, top, bottom};
    }
};

}// namespace types
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_TYPES_PADDING_HPP*/
