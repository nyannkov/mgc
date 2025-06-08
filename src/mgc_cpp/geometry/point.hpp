/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_GEOMETRY_POINT_HPP
#define MGC_GEOMETRY_POINT_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace geometry {

struct Point {
    int16_t x = 0;
    int16_t y = 0;

    Point() = default;
    Point(int16_t x, int16_t y) : x(x), y(y) {}

    mgc_point_t to_c() const {
        return mgc_point{ x, y };
    }

    static Point from_c(const mgc_point_t &pt) {
        return Point(pt.x, pt.y);
    }
};

}// namespace geometry
}// namespace mgc

#endif/*MGC_GEOMETRY_POINT_HPP*/
