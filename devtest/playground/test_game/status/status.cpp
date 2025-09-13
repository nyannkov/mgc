#include <cstdio>
#include "status.hpp"
#include "resources/generated/tileset/tileset_items.h"
#include "resources/generated/font/misaki_gothic.h"

namespace app {

static constexpr char FormatFloor[] = "FLOOR: %d";
static constexpr char FormatGold[]  = "GOLD : %d";

Status::Status(GameContext& ctx)
    : player_(ctx.player()),
      player_stats_(ctx.player_stats()),
      stage_info_(ctx.stage_info()) {

    bool success = false;

    this->clear_cell_drawables();

    label_life_.set_position({0, 0});
    label_life_.set_font(misaki_gothic);
    label_life_.set_fontsize2x(false);
    label_life_.set_font_fore_color(MGC_COLOR_WHITE);
    label_life_.set_text("LIFE: ");
    success = this->add_cell_drawable(label_life_);
    assert(success);

    label_floor_.set_position({0, 10});
    label_floor_.set_font(misaki_gothic);
    label_floor_.set_fontsize2x(false);
    label_floor_.set_font_fore_color(MGC_COLOR_WHITE);
    std::snprintf(floor_txt_, sizeof(floor_txt_), FormatFloor, 0);
    label_floor_.set_text(floor_txt_);
    success = this->add_cell_drawable(label_floor_);
    assert(success);

    label_gold_.set_position({0, 20});
    label_gold_.set_font(misaki_gothic);
    label_gold_.set_fontsize2x(false);
    label_gold_.set_font_fore_color(MGC_COLOR_WHITE);
    std::snprintf(gold_txt_, sizeof(gold_txt_), FormatGold, 0);
    label_gold_.set_text(gold_txt_);
    success = this->add_cell_drawable(label_gold_);
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
    disp_floor_ = stage_info_.current_floor();
    disp_gold_ = player_stats_.gold();

    this->set_hearts(disp_player_hp_);

    updated_ = true;
}

void Status::update() {

    if ( disp_player_hp_ != player_.hp() ) {
        disp_player_hp_ = player_.hp();
        updated_ = true;
        this->set_hearts(disp_player_hp_);
    }

    if ( disp_floor_ != stage_info_.current_floor() ) {
        disp_floor_ = stage_info_.current_floor();
        std::snprintf(floor_txt_, sizeof(floor_txt_), FormatFloor, disp_floor_);
        updated_ = true;
    }

    if ( disp_gold_ != player_stats_.gold() ) {
        disp_gold_ = player_stats_.gold();
        std::snprintf(gold_txt_, sizeof(gold_txt_), FormatGold, disp_gold_);
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
