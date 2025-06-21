/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_BASIC_SPRITE_HPP
#define MGC_PARTS_BASIC_SPRITE_HPP

#include "mgc/components/sprite.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/parts/interfaces/isprite.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace parts {

struct BasicSprite : mgc::parts::interfaces::ISprite<BasicSprite>,
                     mgc::features::Resettable,
                     mgc::features::HasId,
                     mgc::features::HasPosition<mgc::math::Vec2i>,
                     mgc::features::HasParallaxFactor,
                     mgc::features::Visible,
                     mgc::features::Drawable,
                     mgc::features::CellDrawable {

    BasicSprite() { reset(); }
    ~BasicSprite() = default;
    BasicSprite(const BasicSprite&) = delete;
    BasicSprite& operator=(const BasicSprite&) = delete;
    BasicSprite(BasicSprite&&) = default;
    BasicSprite& operator=(BasicSprite&&) = default;

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

    // [impl] WithTileset
    void set_tileset_impl(const mgc::parts::assets::Tileset &tileset);
    const mgc::parts::assets::Tileset *tileset_impl() const;

    // [impl] WithTileIndexing
    void set_tile_index_impl(size_t idx);
    size_t tile_index_impl() const;

    // [impl] WithTrimRegion
    void set_trim_region_impl(mgc::parts::types::TrimRegion region);
    mgc::parts::types::TrimRegion trim_region_impl() const;
    void clear_trim_region_impl();
    bool has_trim_region_impl() const;

private:
    mgc_sprite_t sprite_;
};

}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_BASIC_SPRITE_HPP*/
