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

    bool transfer_region_blocking(const uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        return static_cast<Derived*>(this)->transfer_region_blocking_impl(buffer, len, x0, y0, x1, y1);
    }

    bool transfer_region_async_aligned(const uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        return static_cast<Derived*>(this)->transfer_region_async_aligned_impl(buffer, len, x0, y0, x1, y1);
    }

    bool transfer_full_region_blocking(const uint8_t *buffer, size_t len) {
        return static_cast<Derived*>(this)->transfer_full_region_blocking_impl(buffer, len);
    }

    bool transfer_full_region_async(const uint8_t *buffer, size_t len) {
        return static_cast<Derived*>(this)->transfer_full_region_async_impl(buffer, len);
    }

    bool is_busy() const {
        return static_cast<const Derived*>(this)->is_busy_impl();
    }

    void wait_until_idle_interrupt() {
        static_cast<Derived*>(this)->wait_until_idle_interrupt_impl();
    }

    void wait_until_idle_polling() {
        static_cast<Derived*>(this)->wait_until_idle_polling_impl();
    }

    // Reserved for future use. Unused by core modules. Interface may change.
    void set_on_transfer_async_completed(void (*cb)(void* context), void* ctx) {
        static_cast<Derived*>(this)->set_on_transfer_async_completed_impl(cb, ctx);
    }

protected:
    DisplayDriver() = default;
};

}// namespace display
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_DISPLAY_IDISPLAY_DRIVER_HPP*/
