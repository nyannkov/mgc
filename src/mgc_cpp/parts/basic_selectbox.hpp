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
#include "mgc_cpp/parts/interfaces/iselectbox.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/positionable.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/item_selectable.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace parts {

struct BasicSelectbox : mgc::parts::interfaces::ISelectbox<BasicSelectbox, const char*>,
                        mgc::features::HasId,
                        mgc::features::Positionable,
                        mgc::features::HasParallaxFactor,
                        mgc::features::ItemSelectable,
                        mgc::features::Visible,
                        mgc::features::Drawable,
                        mgc::features::CellDrawable {

    BasicSelectbox() { reset(); }
    ~BasicSelectbox() = default;
    void reset();

    // [feature] HasId
    void set_id(mgc_id_t id) override;
    mgc_id_t get_id() const override;

    // [feature] Positionable
    using mgc::features::Positionable::set_position;
    mgc::geometry::Point get_position() const override;
    void set_position(int16_t x, int16_t y) override;

    // [feature] HasParallaxFactor
    void set_parallax_factor(const mgc::parts::types::ParallaxFactor &factor) override;
    mgc::parts::types::ParallaxFactor get_parallax_factor() const override;

    // [feature] ItemSelectable
    void set_selected_index(int32_t index) override;
    int32_t get_selected_index() const override;
    void select_next() override;
    void select_previous() override;

    //// ItemSelectable-related
    void set_cursor_symbol(const char *symbol);
    const char * get_cursor_symbol() const;
    void set_cursor_offset(int16_t x, int16_t y);
    void set_cursor_offset(mgc::geometry::Point offset);
    mgc::geometry::Point get_cursor_offset() const;

    // [feature] Visible
    bool is_visible() const override;
    void set_visible(bool v) override;

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [feature] CellDrawable
    using mgc::features::CellDrawable::draw;
    bool draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [impl] WithSize
    mgc::parts::types::Size get_size_impl() const;
    void set_size_impl(uint16_t width, uint16_t height);

    //// WithSize-related 
    void adjust_size_to_fit();

    // [impl] WithPadding
    void set_padding_impl(mgc::parts::types::Padding padding);
    mgc::parts::types::Padding get_padding_impl() const;

    // [impl] WithFont
    void set_font_impl(const mgc::parts::assets::Font& font);
    const mgc::parts::assets::Font *get_font_impl() const;
    void set_fore_color_impl(mgc::graphics::Color color);
    mgc::graphics::Color get_fore_color_impl() const;

    //// WithFont-related
    void set_fontsize2x(bool fontsize2x);

    // [impl] WithItems
    void add_item_impl(const char* item);
    void remove_item_by_index_impl(int32_t index);
    void clear_items_impl();
    size_t get_item_count_impl() const;

private:
    mgc_selectbox_t selectbox_;
};

}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_BASIC_SELECTBOX_HPP*/
