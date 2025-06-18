/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_BASIC_SELECTBOX_HPP
#define MGC_PARTS_BASIC_SELECTBOX_HPP

#include "mgc/components/selectbox.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/parts/interfaces/iselectbox.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/item_selectable.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace parts {

struct BasicSelectbox : mgc::parts::interfaces::ISelectbox<BasicSelectbox, const char*>,
                        mgc::features::Resettable,
                        mgc::features::HasId,
                        mgc::features::HasPosition<mgc::math::Vec2i>,
                        mgc::features::HasParallaxFactor,
                        mgc::features::ItemSelectable,
                        mgc::features::Visible,
                        mgc::features::Drawable,
                        mgc::features::CellDrawable {

    BasicSelectbox() { reset(); }
    ~BasicSelectbox() = default;

    // [feature] Resettable
    void reset() override;

    // [feature] HasId
    void set_id(mgc_id_t id) override;
    mgc_id_t id() const override;

    // [feature] HasPosition
    mgc::math::Vec2i position() const override;
    void set_position(const mgc::math::Vec2i& position) override;

    // [feature] HasParallaxFactor
    void set_parallax_factor(const mgc::parts::types::ParallaxFactor &factor) override;
    mgc::parts::types::ParallaxFactor parallax_factor() const override;

    // [feature] ItemSelectable
    void set_selected_index(int32_t index) override;
    int32_t selected_index() const override;
    void select_next() override;
    void select_previous() override;

    //// ItemSelectable-related
    void set_cursor_symbol(const char *symbol);
    const char * cursor_symbol() const;
    void set_cursor_offset(int16_t x, int16_t y);
    void set_cursor_offset(mgc::math::Vec2i offset);
    mgc::math::Vec2i cursor_offset() const;

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

    // [impl] WithPadding
    void set_padding_impl(mgc::parts::types::Padding padding);
    mgc::parts::types::Padding padding_impl() const;

    // [impl] WithFont
    void set_font_impl(const mgc::parts::assets::Font& font);
    const mgc::parts::assets::Font *font_impl() const;
    void set_font_fore_color_impl(mgc::graphics::Color color);
    mgc::graphics::Color font_fore_color_impl() const;

    //// WithFont-related
    void set_fontsize2x(bool fontsize2x);

    // [impl] WithItems
    void add_item_impl(const char* item);
    void remove_item_by_index_impl(int32_t index);
    void clear_items_impl();
    size_t item_count_impl() const;

private:
    mgc_selectbox_t selectbox_;
};

}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_BASIC_SELECTBOX_HPP*/
