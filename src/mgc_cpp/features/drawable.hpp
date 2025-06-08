/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_DRAWABLE_HPP
#define MGC_FEATURES_DRAWABLE_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/graphics/framebuffer.hpp"
#include "mgc_cpp/geometry/point.hpp"
#include "mgc_cpp/parts/types/types.hpp"

namespace mgc {
namespace features {

struct Drawable {

    virtual ~Drawable() = default;

    virtual bool draw(mgc::graphics::Framebuffer &fb, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const = 0;

    bool draw(mgc::graphics::Framebuffer &fb) const {
        return draw(fb, mgc::geometry::Point{0, 0}, nullptr);
    }

    bool draw(mgc::graphics::Framebuffer &fb, const mgc::parts::types::DrawOptions &options) const {
        return draw(fb, mgc::geometry::Point{0, 0}, &options);
    }
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_DRAWABLE_HPP*/
