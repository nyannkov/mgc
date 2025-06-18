/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_INTERFACES_MIXINS_WITH_FONT_HPP
#define MGC_PARTS_INTERFACES_MIXINS_WITH_FONT_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/assets/font.hpp"
#include "mgc_cpp/graphics/color.hpp"

namespace mgc {
namespace parts {
namespace interfaces {
namespace mixins {

template <typename Derived>
struct WithFont {

    void set_font(const mgc::parts::assets::Font& font) {
        static_cast<Derived*>(this)->set_font_impl(font);
    }

    const mgc::parts::assets::Font *font() const {
        return static_cast<const Derived*>(this)->font();
    }

    void set_font_fore_color(mgc::graphics::Color color) {
        static_cast<Derived*>(this)->set_font_fore_color_impl(color);
    }

    mgc::graphics::Color font_fore_color() const {
        return static_cast<const Derived*>(this)->font_fore_color_impl();
    }
};

}// namespace mixins
}// namespace interfaces
}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_INTERFACES_MIXINS_WITH_FONT_HPP*/
