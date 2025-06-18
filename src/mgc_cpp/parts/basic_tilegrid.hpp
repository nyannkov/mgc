/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_BASIC_TILEGRID_HPP
#define MGC_PARTS_BASIC_TILEGRID_HPP

#include "mgc/components/tilemap.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/parts/interfaces/itilegrid.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/positionable.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace parts {

struct IBasicTilegridListener {
    virtual ~IBasicTilegridListener() = default;
    virtual uint8_t on_request_tile_id(uint8_t tile_id, uint16_t row, uint16_t col) { return tile_id; }
};

struct BasicTilegrid : mgc::parts::interfaces::ITilegrid<BasicTilegrid>,
                       mgc::features::HasId,
                       mgc::features::Positionable,
                       mgc::features::HasParallaxFactor,
                       mgc::features::Visible,
                       mgc::features::Drawable,
                       mgc::features::CellDrawable {

    BasicTilegrid() { reset(); }
    ~BasicTilegrid() = default;


    void bind_listener(IBasicTilegridListener& listener);
    void unbind_listener();

    // [feature] Resettable
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

    // [feature] Visible
    bool is_visible() const override;
    void set_visible(bool v) override;

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [feature] CellDrawable
    using mgc::features::CellDrawable::cell_draw;
    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override;

    // [impl] WithTileset
    void set_tileset_impl(const mgc::parts::assets::Tileset &tileset);
    const mgc::parts::assets::Tileset *get_tileset_impl() const;

    // [impl] WithTileIdMap
    void set_tile_id_map_impl(const mgc::parts::assets::TileIdMap &tile_id_map);
    const mgc::parts::assets::TileIdMap *get_tile_id_map_impl() const;

private:
    mgc_tilemap_t tilemap_;
    mgc_tilemap_callbacks_t callbacks_;
    IBasicTilegridListener *listener_;
    static uint8_t on_request_tile_id_wrapper(uint8_t tile_id, uint16_t row, uint16_t col, void *context) {
        auto* obj = static_cast<BasicTilegrid*>(context);
        if ( !obj->listener_ ) {
            return tile_id;
        }
        return obj->listener_->on_request_tile_id(tile_id, row, col);
    }
};

}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_BASIC_TILEMAP_HPP*/
