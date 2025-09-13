/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_RENDER_CELL_RENDERER_HPP
#define MGC_RENDER_CELL_RENDERER_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/platform/display/display_driver.hpp"
#include "mgc_cpp/camera/icamera_follower.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace render {

template <typename DisplayDriverT>
struct CellRenderer {

    static_assert(std::is_base_of<mgc::platform::display::DisplayDriver<DisplayDriverT>, DisplayDriverT>::value,
              "DisplayDriverT must inherit from DisplayDriver<DisplayDriverT>");

    CellRenderer(mgc::graphics::CellBuffer& cb, DisplayDriverT& driver, const mgc::camera::ICameraFollower* follower)
        : cb_(cb), driver_(driver), follower_(follower) { }

    ~CellRenderer() = default;

    CellRenderer(const CellRenderer&) = delete;
    CellRenderer& operator=(const CellRenderer&) = delete;
    CellRenderer(CellRenderer&&) = default;
    CellRenderer& operator=(CellRenderer&&) = default;

    void set_follower(const mgc::camera::ICameraFollower* follower) {
        follower_ = follower;
    }

    void set_back_color(mgc::graphics::Color back_color) {
        cb_.set_back_color(back_color);
    }

    void fill(mgc::graphics::Color color) {
        cb_.fill(color);
    }

    void clear() {
        cb_.clear();
    }

    bool is_busy() const {
        return driver_.is_busy();
    }

    bool draw_to_cell_buffer(
        uint16_t cell_row,
        uint16_t cell_col,
        const mgc::features::CellDrawable* const* drawables,
        size_t drawables_count,
        bool clear_before_draw = true
    ) {
        if ( !drawables ) {
            return false;
        }

        uint16_t x0 = MGC_CELL2PIXEL(cell_col);
        uint16_t y0 = MGC_CELL2PIXEL(cell_row);
        uint16_t x1 = x0 + MGC_CELL_LEN - 1;
        uint16_t y1 = y0 + MGC_CELL_LEN - 1;
        
        auto cam_pos = this->camera_position();

        if ( clear_before_draw ) {
            cb_.clear();
        }

        for ( size_t index = 0; index < drawables_count; index++ ) {
            if ( drawables[index] ) {
                drawables[index]->cell_draw(cb_, x0, y0, cam_pos, nullptr);
            }
        }

        return true;
    }

    bool transfer_to_display_cell_blocking(uint16_t cell_row, uint16_t cell_col, uint16_t offset_x = 0, uint16_t offset_y = 0) {

        uint16_t x0 = MGC_CELL2PIXEL(cell_col) + offset_x;
        uint16_t y0 = MGC_CELL2PIXEL(cell_row) + offset_y;
        uint16_t x1 = x0 + MGC_CELL_LEN - 1;
        uint16_t y1 = y0 + MGC_CELL_LEN - 1;
        uint16_t display_width = driver_.width();
        uint16_t display_height = driver_.height();

        if ( ( display_width <= x1 ) || ( display_height <= y1 ) ) {
            return false;
        }

        driver_.transfer_region_blocking(cb_.data_bytes(), cb_.size(), x0, y0, x1, y1);

        return true;
    }

    bool draw_all_cells_and_transfer(
        const mgc::features::CellDrawable* const* drawables,
        size_t drawables_count,
        bool clear_before_draw = true
    ) {

        if ( !drawables ) {
            return false;
        }

        auto cam_pos = this->camera_position();

        uint16_t display_width_aligned = (driver_.width()>>4)<<4;
        uint16_t display_height_aligned = (driver_.height()>>4)<<4;

        bool all_success = true;
        for ( uint16_t y = 0; y < display_height_aligned; y += MGC_CELL_LEN) {
            for ( uint16_t x = 0; x < display_width_aligned; x += MGC_CELL_LEN) {

                if ( clear_before_draw ) {
                    cb_.clear();
                }

                for ( size_t index = 0; index < drawables_count; index++ ) {
                    if ( drawables[index] ) {
                        drawables[index]->cell_draw(cb_, x, y, cam_pos, nullptr);
                    }
                }

                bool success = driver_.transfer_region_blocking(
                            cb_.data_bytes(),
                            cb_.size(),
                            x,
                            y,
                            x + MGC_CELL_LEN - 1,
                            y + MGC_CELL_LEN - 1 
                );

                all_success = all_success && success;
            }
        }

        return all_success;
    }

    const mgc::graphics::CellBuffer& cell_buffer() const {
        return cb_;
    }

    mgc::graphics::CellBuffer& cell_buffer() {
        return cb_;
    }

private:
    mgc::graphics::CellBuffer& cb_;
    DisplayDriverT& driver_;
    const mgc::camera::ICameraFollower* follower_;

    mgc::math::Vec2i camera_position() {
        if ( follower_ ) {
            return follower_->follow_position();
        } else {
            return mgc::math::Vec2i(0, 0);
        }
    }
};

}// namespace render
}// namespace mgc

#endif/*MGC_RENDER_CELL_RENDERER_HPP*/
