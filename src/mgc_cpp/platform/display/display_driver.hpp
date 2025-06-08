/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PLATFORM_DISPLAY_DISPLAY_DRIVER_HPP
#define MGC_PLATFORM_DISPLAY_DISPLAY_DRIVER_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace platform {
namespace display {

template <typename Derived>
struct DisplayDriver {

    void init() {
        static_cast<Derived*>(this)->init_impl();
    }

    uint16_t width() const {
        return static_cast<const Derived*>(this)->width_impl();
    }

    uint16_t height() const {
        return static_cast<const Derived*>(this)->height_impl();
    }

    void transfer_region(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        static_cast<Derived*>(this)->transfer_region_impl(buffer, len, x0, y0, x1, y1);
    }

protected:
    DisplayDriver() = default;
};

}// namespace input
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_DISPLAY_IDISPLAY_DRIVER_HPP*/
