/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_STATUS_SCREEN_HPP
#define MGC_STATUS_SCREEN_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/tileset/tileset_items.h"
#include "game_context.hpp"
#include "app_typedefs.hpp"

namespace app {

template <
    uint16_t X,
    uint16_t Y,
    uint16_t Width,
    uint16_t Height,
    typename DisplayDriverT
>
struct StatusScreen {
    using ColorT = mgc::graphics::Color;
    using CellBufferT = mgc::graphics::CellBuffer;
    using CellRendererT = mgc::render::CellRenderer<DisplayDriverT>;
    using UpdatableT = mgc::features::Updatable<GameContext>;

    explicit StatusScreen(DisplayDriverT& display_driver)
        : cell_renderer_(cb_, display_driver, nullptr) {

        heart1_.set_tileset(tileset_items);
        heart1_.set_tile_index(2);
        heart1_.set_visible(true);
        heart1_.set_position(mgc::math::Vec2i(0, 0));
        heart1_.set_parallax_factor(mgc::graphics::ParallaxFactor(0.0f, 0.0f));

        heart2_.set_tileset(tileset_items);
        heart2_.set_tile_index(2);
        heart2_.set_visible(true);
        heart2_.set_position(mgc::math::Vec2i(8, 0));
        heart1_.set_parallax_factor(mgc::graphics::ParallaxFactor(0.0f, 0.0f));

        twin_hearts_[0] = static_cast<mgc::features::CellDrawable*>(&heart1_);
        twin_hearts_[1] = static_cast<mgc::features::CellDrawable*>(&heart2_);

        cell_renderer_.cell_buffer().set_back_color(MGC_COLOR_BLACK);
        cell_renderer_.cell_buffer().clear();
    }

    ~StatusScreen() = default;
    StatusScreen(const StatusScreen&) = delete;
    StatusScreen& operator=(const StatusScreen&) = delete;
    StatusScreen(StatusScreen&&) = default;
    StatusScreen& operator=(StatusScreen&&) = default;

    void update(const GameContext& ctx) {
        if ( ctx.player_life() != player_life_value_ ) {
            is_update_life_value_ = true;
            player_life_value_ = ctx.player_life();
        } else {
            is_update_life_value_ = false;
        }
    }

    void draw_blocking_if_needed() {

        if ( !is_update_life_value_) {
            return;
        }

        while ( cell_renderer_.is_busy() );

        auto draw_life = player_life_value_;

        for ( uint16_t row = 0; row < Rows; row++ ) {
            for ( uint16_t col = 0; col < Cols; col++ ) {

                if ( 2 <= draw_life ) {
                    heart1_.set_tile_index(1);
                    heart2_.set_tile_index(1);
                    draw_life -= 2;
                } else if ( 1 == draw_life ) {
                    heart1_.set_tile_index(1);
                    heart2_.set_tile_index(2);
                    draw_life -= 1;
                } else {
                    heart1_.set_tile_index(2);
                    heart2_.set_tile_index(2);
                }
                
                cell_renderer_.draw_to_cell_buffer(
                    0, 0, twin_hearts_.data(), twin_hearts_.size()
                );

                cell_renderer_.transfer_to_display_cell_blocking(row, col, 8, 8);
            }
        }
    }


private:
    static constexpr uint16_t Rows = Height / MGC_CELL_LEN;
    static constexpr uint16_t Cols = Width / MGC_CELL_LEN;
    CellBufferT cb_;
    CellRendererT cell_renderer_;
    bool is_update_life_value_;
    size_t player_life_value_;
    mgc::parts::BasicSprite heart1_;
    mgc::parts::BasicSprite heart2_;
    std::array<const mgc::features::CellDrawable*, 2> twin_hearts_;
};

} // namepsace app

#endif/*MGC_STATUS_SCREEN_HPP*/

