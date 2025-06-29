/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_CELL_DRAWABLE_HPP
#define MGC_FEATURES_CELL_DRAWABLE_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/graphics/cell_buffer.hpp"
#include "mgc_cpp/graphics/draw_options.hpp"
#include "mgc_cpp/math/vec2.hpp"

namespace mgc {
namespace features {

struct CellDrawable {

    virtual ~CellDrawable() = default;

    virtual bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const = 0;

    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y) const {

        return cell_draw(cb, cell_x, cell_y, mgc::math::Vec2i(0, 0), nullptr);
    }

    bool cell_draw(graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::graphics::DrawOptions &options) const {

        return cell_draw(cb, cell_x, cell_y, mgc::math::Vec2i(0, 0), &options);
    }
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_CELL_DRAWABLE_HPP*/
