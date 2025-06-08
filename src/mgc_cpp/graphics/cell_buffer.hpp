/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_GRAPHICS_CELL_BUFFER_HPP
#define MGC_GRAPHICS_CELL_BUFFER_HPP

#include "mgc/render/pixelbuffer.h"
#include "mgc_cpp/internal/common.hpp"
#include "color.hpp"

namespace mgc {
namespace graphics {

struct CellBuffer {

    CellBuffer() {
        pixelbuffer_init(&buffer_);
    }

    ~CellBuffer() = default;

    CellBuffer(const CellBuffer&) = delete;

    CellBuffer& operator=(const CellBuffer&) = delete;

    CellBuffer(CellBuffer&&) = default;

    CellBuffer& operator=(CellBuffer&&) = default;

    Color* data() {
        return pixelbuffer_get_buffer_mut(&buffer_);
    }

    uint8_t* data_bytes() {
        return reinterpret_cast<uint8_t*>(pixelbuffer_get_buffer_mut(&buffer_));
    }

    const Color* data() const {
        return pixelbuffer_get_buffer(&buffer_);
    }

    const uint8_t* data_bytes() const {
        return reinterpret_cast<const uint8_t*>(pixelbuffer_get_buffer(&buffer_));
    }

    uint16_t width() const {
        return pixelbuffer_get_width(&buffer_);
    }

    uint16_t height() const {
        return pixelbuffer_get_height(&buffer_);
    }

    size_t size() const {
        return sizeof(Color) * width() * height();
    }

    void set_back_color(Color color) {
        pixelbuffer_set_back_color(&buffer_, color);
    }

    void clear() {
        pixelbuffer_reset_buffer(&buffer_);
    }

private:
    mgc_pixelbuffer buffer_;
};

}// namespace graphics
}// namespace mgc

#endif/*MGC_GRAPHICS_CELL_BUFFER_HPP*/
