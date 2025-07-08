/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_GRAPHICS_DOUBLE_FRAMEBUFFER_HPP
#define MGC_GRAPHICS_DOUBLE_FRAMEBUFFER_HPP

#include "framebuffer.hpp"

namespace mgc {
namespace graphics {

struct DoubleFramebuffer {

    DoubleFramebuffer(Color* buf0, Color* buf1, uint16_t w, uint16_t h)
        : fb0_(buf0, w, h),
          fb1_(buf1, w, h),
          draw_fb_(&fb0_),
          transfer_fb_(&fb1_)
    {}

    DoubleFramebuffer(const DoubleFramebuffer&) = delete;
    DoubleFramebuffer& operator=(const DoubleFramebuffer&) = delete;
    DoubleFramebuffer(DoubleFramebuffer&&) = default;
    DoubleFramebuffer& operator=(DoubleFramebuffer&&) = default;

    void set_back_color(Color color) {
        fb0_.set_back_color(color);
        fb1_.set_back_color(color);
    }

    Framebuffer& back() {
        return *draw_fb_;
    }

    const Framebuffer& front() const {
        return *transfer_fb_;
    }

    void swap() {
        std::swap(draw_fb_, transfer_fb_);
    }

private:
    Framebuffer fb0_;
    Framebuffer fb1_;
    Framebuffer* draw_fb_;
    Framebuffer* transfer_fb_;
};

} // namespace graphics
} // namespace mgc

#endif // MGC_GRAPHICS_DOUBLE_FRAMEBUFFER_HPP
