#include <cstdio>
#include "status.hpp"
#include "resources/generated/tileset/tileset_items.h"
#include "resources/generated/font/misaki_gothic.h"

namespace app {

static constexpr char FormatGold[]  = "GYOLD : %d";

Status::Status(GameContext& ctx)
    : player_(ctx.world_state.player) {

    bool success = false;

    this->clear_cell_drawables();

    label_life_.set_position({0, 0});
    label_life_.set_font(misaki_gothic);
    label_life_.set_fontsize2x(false);
    label_life_.set_font_fore_color(MGC_COLOR_WHITE);
    label_life_.set_text("LIFE: ");
    success = this->add_cell_drawable(label_life_);
    assert(success);

    label_money_.set_position({0, 10});
    label_money_.set_font(misaki_gothic);
    label_money_.set_fontsize2x(false);
    label_money_.set_font_fore_color(MGC_COLOR_WHITE);

    disp_money_ = player_.money();
    std::snprintf(money_txt_, sizeof(money_txt_), FormatGold, disp_money_);
    label_money_.set_text(money_txt_);
    success = this->add_cell_drawable(label_money_);
    assert(success);

    for ( size_t i = 0; i < hearts_.size(); i++ ) {
        auto& h = hearts_[i];
        h.set_tileset(tileset_items);
        h.set_tile_index(2);
        h.set_position(mgc::math::Vec2i(4*6 + 8*i, 0));
        h.set_parallax_factor(mgc::graphics::ParallaxFactor(0.0f, 0.0f));

        if ( i < player_.full_hp() ) {
            h.set_visible(true);
        } else {
            h.set_visible(false);
        }

        success = this->add_cell_drawable(h);
        assert(success);
    }
    
    disp_player_hp_ = player_.hp();
    this->set_hearts(disp_player_hp_);

    updated_ = true;
}

void Status::update() {

    if ( disp_player_hp_ != player_.hp() ) {
        disp_player_hp_ = player_.hp();
        updated_ = true;
        this->set_hearts(disp_player_hp_);
    }

    if ( disp_money_ != player_.money() ) {
        disp_money_ = player_.money();
        std::snprintf(money_txt_, sizeof(money_txt_), FormatGold, disp_money_);
        updated_ = true;
    }
}

void Status::set_hearts(int32_t player_hp) {
    for ( size_t i = 0; i < hearts_.size(); i++ ) {
        auto& h = hearts_[i];
        if ( i < player_.full_hp() ) {
            if ( i < player_hp ) {
                h.set_tile_index(1);
            } else {
                h.set_tile_index(2);
            }
            h.set_visible(true);
        } else {
            h.set_visible(false);
        }
    }
}

bool Status::add_cell_drawable(mgc::features::CellDrawable& obj) {
    
    if ( count_cell_drawables_ >= cell_drawables_.size() ) {
        return false;
    }

    cell_drawables_[count_cell_drawables_] = &obj;
    count_cell_drawables_++;
    return true;
}

void Status::clear_cell_drawables() {
    for ( auto& p : cell_drawables_ ) {
        p = nullptr;
    }
    count_cell_drawables_ = 0; 
}

} // namepsace app
