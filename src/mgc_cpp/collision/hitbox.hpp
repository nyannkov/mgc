/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_HITBOX_HPP
#define MGC_COLLISION_HITBOX_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"

namespace mgc {
namespace collision {

using HitboxOffset = mgc::math::Vec2<int16_t>;
struct HitboxSize {
    using ValueType = uint16_t;
    constexpr HitboxSize() : size_(0, 0) {}
    constexpr HitboxSize(ValueType w, ValueType h)
        : size_{w, h} {}

    constexpr ValueType width() const { return size_.x; }
    constexpr ValueType height() const { return size_.y; }

    constexpr mgc::math::Vec2<ValueType> vec() const { return size_; }

private:
    mgc::math::Vec2<ValueType> size_;
};

struct Hitbox {
    constexpr Hitbox()
        : id(0), enabled(false), offset(), size() {}

    constexpr Hitbox(mgc_id_t id_, bool enabled_, HitboxOffset offset_, HitboxSize size_)
        : id(id_), enabled(enabled_), offset(offset_), size(size_) {}

    mgc_id_t id;
    bool enabled;
    HitboxOffset offset;
    HitboxSize size;
};

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_HITBOX_HPP*/
