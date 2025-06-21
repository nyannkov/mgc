/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_TYPES_SIZE_HPP
#define MGC_PARTS_TYPES_SIZE_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace parts {
namespace types {

struct Size {
    uint16_t width = 0;
    uint16_t height = 0;

    constexpr Size() = default;
    constexpr Size(uint16_t width, uint16_t height)
        : width(width), height(height) {}

    explicit Size(const mgc_size_t& data)
        : width(data.width), height(data.height) {}

    constexpr mgc_size_t to_c() const {
        return mgc_size_t{width, height};
    }
};

}// namespace types
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_TYPES_SIZE_HPP*/
