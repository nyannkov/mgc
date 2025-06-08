/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_GRAPHICS_FRAMEBUFFER_HPP
#define MGC_GRAPHICS_FRAMEBUFFER_HPP

#include "mgc/render/framebuffer.h"
#include "mgc_cpp/internal/common.hpp"
#include "color.hpp"

namespace mgc {
namespace graphics {

struct Framebuffer {

    Framebuffer(Color* buffer, uint16_t width, uint16_t height) {
        framebuffer_init(&fb_, buffer, width, height);
    }

    ~Framebuffer() = default;

    Framebuffer(const Framebuffer&) = delete;

    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&&) = default;

    Framebuffer& operator=(Framebuffer&&) = default;

    Color* data() {
        return framebuffer_get_buffer_mut(&fb_);
    }

    uint8_t* data_bytes() {
        return reinterpret_cast<uint8_t*>(framebuffer_get_buffer_mut(&fb_));
    }

    const Color* data() const {
        return framebuffer_get_buffer(&fb_);
    }

    const uint8_t* data_bytes() const {
        return reinterpret_cast<const uint8_t*>(framebuffer_get_buffer(&fb_));
    }

    uint16_t width() const {
        return framebuffer_get_width(&fb_);
    }

    uint16_t height() const {
        return framebuffer_get_height(&fb_);
    }

    size_t size() const {
        return sizeof(Color) * width() * height();
    }

    void set_back_color(Color color) {
        back_color_ = color;
    }

    void clear() {
        framebuffer_clear(&fb_, back_color_);
    }

private:
    mgc_framebuffer fb_;
    Color back_color_;
};

}// namespace graphics
}// namespace mgc

#endif/*MGC_GRAPHICS_FRAMEBUFFER_HPP*/
