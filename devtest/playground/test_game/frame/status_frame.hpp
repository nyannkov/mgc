/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_STATUS_FRAME_HPP
#define MGC_STATUS_FRAME_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/tileset/tileset_items.h"
#include "status/status.hpp"
#include "game_context/game_context.hpp"

namespace app {

template <uint16_t Width, uint16_t Height, typename DisplayDriverT>
struct StatusFrame {

    StatusFrame(
        uint16_t x,
        uint16_t y,
        DisplayDriverT& display_driver,
        GameContext& ctx
    ) : x_(x),
       y_(y),
       cell_renderer_(cb_, display_driver, nullptr),
       status_(ctx) {

        cell_renderer_.cell_buffer().set_back_color(MGC_COLOR_BLACK);
        cell_renderer_.cell_buffer().clear();
    }

    ~StatusFrame() = default;
    StatusFrame(const StatusFrame&) = delete;
    StatusFrame& operator=(const StatusFrame&) = delete;
    StatusFrame(StatusFrame&&) = default;
    StatusFrame& operator=(StatusFrame&&) = default;

    void update() {
        status_.update();
    }

    void clear_display_blocking(mgc::graphics::Color color = MGC_COLOR_BLACK) {

        cell_renderer_.fill(color);

        for ( uint16_t row = 0; row < Rows; row++ ) {
            for ( uint16_t col = 0; col < Cols; col++ ) {
                cell_renderer_.transfer_to_display_cell_blocking(row, col, x_, y_);
            }
        }
        cell_renderer_.clear();
    }

    bool has_update() const {
        return status_.is_updated();
    }

    void clear_update() {
        status_.clear_updated();
    }

    void draw_blocking() {

        auto cell_drawables = status_.cell_drawables();
        size_t count = status_.cell_drawables_count();
        
        for ( uint16_t row = 0; row < Rows; row++ ) {
            for ( uint16_t col = 0; col < Cols; col++ ) {

                cell_renderer_.draw_to_cell_buffer(row, col, cell_drawables, count);
                cell_renderer_.transfer_to_display_cell_blocking(row, col, x_, y_);
            }
        }
    }


private:
    static constexpr uint16_t Rows = Height / MGC_CELL_LEN;
    static constexpr uint16_t Cols = Width / MGC_CELL_LEN;
    uint16_t x_;
    uint16_t y_;
    mgc::graphics::CellBuffer cb_;
    mgc::render::CellRenderer<DisplayDriverT> cell_renderer_;
    Status status_;
};

} // namepsace app

#endif/*MGC_STATUS_FRAME_HPP*/

