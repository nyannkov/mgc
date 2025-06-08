/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_POSITIONABLE_HPP
#define MGC_FEATURES_POSITIONABLE_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/geometry/point.hpp"

namespace mgc {
namespace features {

struct Positionable {

    virtual ~Positionable() = default;

    virtual mgc::geometry::Point get_position() const = 0;
    virtual void set_position(int16_t x, int16_t y) = 0;
    void set_position(const mgc::geometry::Point &pt) {
        set_position(pt.x, pt.y);
    }
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_POSITIONABLE_HPP*/
