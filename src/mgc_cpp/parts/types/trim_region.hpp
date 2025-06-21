/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_TYPES_TRIM_REGION_HPP
#define MGC_PARTS_TYPES_TRIM_REGION_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace types {

struct TrimRegion {
    uint16_t top = 0;
    uint16_t bottom = 0;
    uint16_t left = 0;
    uint16_t right = 0;

    constexpr TrimRegion() = default;
    constexpr TrimRegion(uint16_t top, uint16_t bottom, uint16_t left, uint16_t right) 
        : top(top), bottom(bottom), left(left), right(right) {}

    explicit TrimRegion(const mgc_trim_region_t& data)
        : top(data.top), bottom(data.bottom), left(data.left), right(data.right) {} 

    constexpr mgc_trim_region_t to_c() const {
        return mgc_trim_region_t{left, right, top, bottom};
    }
};

}// namespace types
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_TYPES_TRIM_REGION_HPP*/
