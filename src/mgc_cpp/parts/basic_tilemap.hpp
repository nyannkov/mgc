/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_BASIC_TILEMAP_HPP
#define MGC_PARTS_BASIC_TILEMAP_HPP

#include "mgc/components/tilemap.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/interfaces/itilemap.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/positionable.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/has_tile_collision_map.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace parts {

struct BasicTilemap : mgc::parts::interfaces::ITilemap<BasicTilemap>,
                      mgc::features::HasId,
                      mgc::features::Positionable,
                      mgc::features::HasParallaxFactor,
                      mgc::features::HasTileCollisionMap,
                      mgc::features::Visible,
                      mgc::features::Drawable,
                      mgc::features::CellDrawable {

    BasicTilemap() { 

        reset();

        callbacks_ = mgc_tilemap_callbacks_t {
            this,
            on_request_tile_id_wrapper
        };
        tilemap_set_callbacks(&tilemap_, &callbacks_);
    }
    ~BasicTilemap() = default;
    void reset();

    virtual uint8_t on_request_tile_id(uint8_t tile_id, uint16_t row, uint16_t col) { return tile_id; }

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

    // [feature] HasTileCollisionMap
    void set_tile_collision_map(const mgc::parts::assets::TileCollisionMap& map) override;
    const mgc::parts::assets::TileCollisionMap * get_tile_collision_map() const override;
    void set_collision_enabled(bool enabled) override;
    bool get_collision_enabled() const override;

    // [feature] Visible
    bool is_visible() const override;
    void set_visible(bool v) override;

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [feature] CellDrawable
    using mgc::features::CellDrawable::draw;
    bool draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [impl] WithTileset
    void set_tileset_impl(const mgc::parts::assets::Tileset &tileset);
    const mgc::parts::assets::Tileset *get_tileset_impl() const;

private:
    mgc_tilemap_t tilemap_;
    mgc_tilemap_callbacks_t callbacks_;
    static uint8_t on_request_tile_id_wrapper(uint8_t tile_id, uint16_t row, uint16_t col, void *context) {
        auto obj = static_cast<mgc::parts::BasicTilemap *>(context);
        return obj->on_request_tile_id(tile_id, row, col);
    }
};

}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_BASIC_TILEMAP_HPP*/
