/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_BASIC_LABEL_HPP
#define MGC_PARTS_BASIC_LABEL_HPP

#include "mgc/components/label.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/parts/interfaces/ilabel.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace parts {

struct BasicLabel : mgc::parts::interfaces::ILabel<BasicLabel>,
                    mgc::features::Resettable,
                    mgc::features::HasId,
                    mgc::features::HasPosition<mgc::math::Vec2i>,
                    mgc::features::HasParallaxFactor,
                    mgc::features::Visible,
                    mgc::features::Drawable,
                    mgc::features::CellDrawable {

    BasicLabel() { reset(); }
    ~BasicLabel() = default;
    void reset();

    // [feature] HasId
    void set_id(mgc_id_t id) override;
    mgc_id_t id() const override;

    // [feature] HasPosition
    mgc::math::Vec2i position() const override;
    void set_position(const mgc::math::Vec2i& position) override;

    // [feature] HasParallaxFactor
    void set_parallax_factor(const mgc::parts::types::ParallaxFactor &factor) override;
    mgc::parts::types::ParallaxFactor parallax_factor() const override;

    // [feature] Visible
    bool is_visible() const override;
    void set_visible(bool v) override;

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [feature] CellDrawable
    using mgc::features::CellDrawable::cell_draw;
    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::math::Vec2i &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [impl] WithSize
    mgc::parts::types::Size size_impl() const;
    void set_size_impl(uint16_t width, uint16_t height);

    //// WithSize-related
    void adjust_size_to_fit();

    // [impl] WithFont
    void set_font_impl(const mgc::parts::assets::Font& font);
    const mgc::parts::assets::Font *font_impl() const;
    void set_font_fore_color_impl(mgc::graphics::Color color);
    mgc::graphics::Color font_fore_color_impl() const;

    //// WithFont-related
    void set_fontsize2x(bool fontsize2x);
    mgc::graphics::Color back_color() const;
    void set_enabled_back_color(bool enabled);
    bool is_back_enabled() const;

    // [impl] WithText
    void set_text_impl(const char *text);
    const char *text_impl() const;
    void clear_text_impl();

private:
    mgc_label_t label_;
};

}// namespace ui
}// namespace mgc

#endif/*MGC_UI_BASIC_LABEL_HPP*/
