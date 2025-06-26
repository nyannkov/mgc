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
    
    void init(uint32_t clock_rate = 0) {
        static_cast<Derived*>(this)->init_impl(clock_rate);
    }

    void reset() {
        static_cast<Derived*>(this)->reset_impl();
    }

    uint16_t width() const {
        return static_cast<const Derived*>(this)->width_impl();
    }

    uint16_t height() const {
        return static_cast<const Derived*>(this)->height_impl();
    }

    void transfer_region_blocking(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        static_cast<Derived*>(this)->transfer_region_blocking_impl(buffer, len, x0, y0, x1, y1);
    }

    void transfer_full_region_blocking(uint8_t *buffer, size_t len) {
        static_cast<Derived*>(this)->transfer_full_region_blocking_impl(buffer, len);
    }

    void transfer_full_region_async(uint8_t *buffer, size_t len) {
        static_cast<Derived*>(this)->transfer_full_region_async_impl(buffer, len);
    }

    bool is_busy() const {
        return static_cast<const Derived*>(this)->is_busy_impl();
    }

protected:
    DisplayDriver() = default;
};

}// namespace display
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_DISPLAY_IDISPLAY_DRIVER_HPP*/
