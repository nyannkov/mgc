/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_BASIC_DIALOGUEBOX_HPP
#define MGC_PARTS_BASIC_DIALOGUEBOX_HPP

#include "mgc/components/dialoguebox.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/parts/interfaces/idialoguebox.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace parts {

struct BasicDialoguebox : mgc::parts::interfaces::IDialoguebox<BasicDialoguebox>,
                          mgc::features::Resettable,
                          mgc::features::HasId,
                          mgc::features::HasPosition<mgc::math::Vec2i>,
                          mgc::features::HasParallaxFactor,
                          mgc::features::Visible,
                          mgc::features::Drawable,
                          mgc::features::CellDrawable {

    BasicDialoguebox() { reset(); }
    ~BasicDialoguebox() = default;
    BasicDialoguebox(const BasicDialoguebox&) = delete;
    BasicDialoguebox& operator=(const BasicDialoguebox&) = delete;
    BasicDialoguebox(BasicDialoguebox&&) = default;
    BasicDialoguebox& operator=(BasicDialoguebox&&) = default;

    // [feature] Resettable
    void reset() override;

    // [feature] HasId
    void set_id(mgc_id_t id) override;
    mgc_id_t id() const override;

    // [feature] HasPosition
    mgc::math::Vec2i position() const override;
    void set_position(const mgc::math::Vec2i& position) override;

    // [feature] HasParallaxFactor
    void set_parallax_factor(const mgc::graphics::ParallaxFactor &factor) override;
    mgc::graphics::ParallaxFactor parallax_factor() const override;

    // [feature] Visible
    bool is_visible() const override;
    void set_visible(bool v) override;

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override;

    // [feature] CellDrawable
    using mgc::features::CellDrawable::cell_draw;
    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override;

    // [impl] WithSize
    mgc::parts::types::Size size_impl() const;
    void set_size_impl(uint16_t width, uint16_t height);

    //// WithSize-related
    void adjust_size_to_fit();

    // [impl] WithPadding
    void set_padding_impl(mgc::parts::types::Padding margin);
    mgc::parts::types::Padding padding_impl() const;

    // [impl] WithFont
    void set_font_impl(const mgc::parts::assets::Font& font);
    const mgc::parts::assets::Font *font_impl() const;
    void set_font_fore_color_impl(mgc::graphics::Color color);
    mgc::graphics::Color font_fore_color_impl() const;

    //// WithFont-related
    void set_fontsize2x(bool fontsize2x);
    mgc::graphics::Color back_color() const;
    bool is_back_enabled() const;

    // [impl] WithText
    void set_text_impl(const char *text);
    const char *text_impl() const;
    void clear_text_impl();

    // [impl] WithAutoTextScroll
    void set_scroll_speed_impl(uint32_t speed);
    uint32_t scroll_speed_impl() const;
    void set_scroll_threshold_line_impl(uint32_t threshold_line);
    uint32_t scroll_threshold_line_impl() const;
    void set_line_spacing_impl(uint32_t line_spacing);
    uint32_t line_spacing_impl() const;

    // [impl] WithTypingEffect
    void set_typing_speed_impl(uint32_t chars_per_frame);
    uint32_t typing_speed_impl() const;
    void advance_typing_impl();
    bool is_typing_complete_impl() const;

private:
    mgc_dialoguebox_t dialoguebox_;
};

}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_BASIC_DIALOGUEBOX_HPP*/
