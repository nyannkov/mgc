/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_ST7789_HPP
#define MGC_DRIVERS_ST7789_HPP

#include "mgc_cpp/platform/display/display_driver.hpp"
#include "mgc_drivers/platform/display/st7789/st7789.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace display {

struct ST7789 : mgc::platform::display::DisplayDriver<ST7789> {

    ST7789() = default;

    ST7789(const ST7789&) = delete;
    ST7789& operator=(const ST7789&) = delete;
    ST7789(ST7789&&) = default;
    ST7789& operator=(ST7789&&) = default;

    void init_impl(uint32_t clock_rate) {
        st7789_init(clock_rate);
    }

    void reset_impl() {
        st7789_reset();
    }

    uint16_t width_impl() const {
        return MGC_DRIVERS_ST7789_WIDTH;
    }

    uint16_t height_impl() const {
        return MGC_DRIVERS_ST7789_HEIGHT;
    }

    void transfer_region_blocking_impl(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        st7789_transfer_region_blocking_rgb565(buffer, len, x0, y0, x1, y1);
    }

    void transfer_full_region_blocking_impl(uint8_t *buffer, size_t len) {
        st7789_transfer_full_region_blocking_rgb565(buffer, len);
    }

    void transfer_full_region_async_impl(uint8_t *buffer, size_t len) {
        st7789_transfer_full_region_async_rgb565(buffer, len);
    }

    bool is_busy_impl() const {
        return st7789_is_busy();
    }

    void display_on() {
        st7789_display_on();
    }

    void display_off() {
        st7789_display_off();
    }

    void inversion_on() {
        st7789_inversion_on();
    }

    void inversion_off() {
        st7789_inversion_off();
    }

    void sleep_in() {
        st7789_sleep_in();
    }

    void sleep_out() {
        st7789_sleep_out();
    }
};

}// namespace display
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_DRIVERS_ST7789_HPP*/
