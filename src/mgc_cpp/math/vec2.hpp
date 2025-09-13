/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_MATH_VEC2_HPP
#define MGC_MATH_VEC2_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace math {

template <typename T>
struct Vec2 {
    T x = 0;
    T y = 0;

    constexpr Vec2() = default;
    constexpr Vec2(T x, T y) : x(x), y(y) {}

    constexpr Vec2 operator+(const Vec2& rhs) const {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    constexpr Vec2 operator-(const Vec2& rhs) const {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    constexpr Vec2& operator+=(const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr Vec2& operator-=(const Vec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    constexpr bool operator==(const Vec2& rhs) const {
        return (x == rhs.x) && (y == rhs.y);
    }

    constexpr bool operator!=(const Vec2& rhs) const {
        return !(*this == rhs);
    }

    constexpr Vec2 operator*(T scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    constexpr Vec2 operator/(T scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    constexpr Vec2& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    constexpr Vec2& operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    template <typename U>
    constexpr Vec2<U> cast_to() const {
        return Vec2<U>{ static_cast<U>(x), static_cast<U>(y) };
    }

    friend constexpr Vec2 operator*(T scalar, const Vec2& v) {
        return v * scalar;
    }
};

using Vec2f = Vec2<float>;
using Vec2i16 = Vec2<int16_t>;
using Vec2i= Vec2i16;

}// namespace math
}// namespace mgc

#endif/*MGC_MATH_VEC2_HPP*/
