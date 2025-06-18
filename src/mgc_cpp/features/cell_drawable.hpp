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
#include "mgc_cpp/geometry/point.hpp"
#include "mgc_cpp/parts/types/types.hpp"

namespace mgc {
namespace features {

struct CellDrawable {

    virtual ~CellDrawable() = default;

    virtual bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const = 0;

    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y) const {

        return cell_draw(cb, cell_x, cell_y, mgc::geometry::Point{0, 0}, nullptr);
    }

    bool cell_draw(graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::parts::types::DrawOptions &options) const {

        return cell_draw(cb, cell_x, cell_y, mgc::geometry::Point{0, 0}, &options);
    }
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_CELL_DRAWABLE_HPP*/
