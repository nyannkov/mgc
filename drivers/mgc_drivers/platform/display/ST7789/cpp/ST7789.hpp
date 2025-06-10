/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_ST7789_HPP
#define MGC_DRIVERS_ST7789_HPP

#include "mgc_cpp/platform/display/display_driver.hpp"
#include "mgc_drivers/platform/display/ST7789/ST7789.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace display {

struct ST7789 : mgc::platform::display::DisplayDriver<ST7789> {

    void init_impl(uint32_t clock_rate) {
        ST7789_init(clock_rate);
    }

    void reset_impl() {
        ST7789_reset();
    }

    uint16_t width_impl() const {
        return ST7789_get_width();
    }

    uint16_t height_impl() const {
        return ST7789_get_height();
    }

    void transfer_region_impl(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        ST7789_transfer_region(buffer, len, x0, y0, x1, y1);
    }

};

}// namespace display
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_DRIVERS_ST7789_HPP*/
