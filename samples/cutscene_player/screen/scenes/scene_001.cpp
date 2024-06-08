/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <cstdio>
#include <memory>
#include "scene_001.hpp"
#include "system/system.h"
#include "resources/generates/map/map_001_l1.h"
#include "resources/generates/map/map_001_l2.h"
#include "resources/generates/map/map_001_l3.h"
#include "resources/generates/tileset/tileset_map.h"
#include "resources/generates/tileset/tileset_title_greensleeves.h"
#include "resources/generates/tileset/tileset_actor_greensleeves.h"

namespace mgc::screen {

// greensleeves
static const char *lyrics_greensleeves[] = {
    [0]="",

    [1]="Alas, my love, you do me wrong,",
    [2]="To cast me off discourteously.",

    [3]="For I have loved you so long,",
    [4]="Delighting in your company.",

    [5]="Greensleeves was all my joy,",
    [6]="Greensleeves was my delight,",

    [7]="Greensleeves was my heart of gold,",
    [8]="And who but my lady greensleeves.",
};

static const mgc_id_t HITBOX_ACTOR_ID = 0;

static int32_t mml_cb_param;
static bool mml_cb_update;

static const mgc_mml_record_t mml_records[] = {
    {
        .id = 0,
        .mml = {
            //header
            ":V1M1;"
            // Channel A (ch = 0)
            "T120"
            "$B90"
            "$E1$A0$H100$D100$S90$F2000"
            "V15O4L4"
            "@C1" "A>C2D"
            "@C2" "E.F8E"
            "@C3" "D2<BG.A8B>C.<B8AG#.F#8G#"
            "@C4" "A2.A2"

            "@C101" "A>C2DE.F8ED2<BG.A8B"
            "@C102" ">C2<AA.G#8AB2G#E2"

            "@C103" "A>C2DE.F8ED2<BG.A8B"
            "@C104" ">C.<B8AG#.F#8G#A2.A2."

            "@C105" ">G2GG.F8ED2<BG.A8B"
            "@C106" ">C2<AA.G#8AB2G#E2."

            "@C107" ">G2GG.F8ED2<BG.A8B"
            "@C108" ">C.<B8AG#.F#8G#A2.A2"

            "@C100" "A>C2DE.F8ED2<BG.A8B >C2<AA.G#8AB2G#E2"
                    "A>C2DE.F8ED2<BG.A8B >C.<B8AG#.F#8G#A2.A2."

            "@C105" ">G2GG.F8ED2<BG.A8B"
            "@C106" ">C2<AA.G#8AB2G#E2."

            "@C107" ">G2GG.F8ED2<BG.A8B"
            "@C108" ">C.<B8AG#.F#8G#A2.A2"
            "@C100"

            "$B92O5"
            "V15"
            "$E1$A20$H100$D100$S80$F800"
            "A>C2DE.F8ED2<BG.A8B >C.<B8AG#.F#8G#A2.A2."
            ","
            // Channel B (ch = 1)
            "T120"
            "$B90"
            "$E1$A0$H100$D100$S90$F2000"
            "V15O3L2"
            "R4"
            "A.>C.<G.G. F.E.A.A."
            "[2 A.>C.<G.G.|F.D.E.E.]F.E.A.A."
            "[2 >C.C.<G.G.|F.D.E.E.]F.E.A.A."
            "A.>C.<G.G. F.D.E.E."
            "A.>C.<G.G. F.E.A.AA4"
            "[2 >C.C.<G.G.|F.D.E.E.]F.E.A.A."
            "$E1$A20$H100$D100$S70$F600"
            "L4O5"
            "<A>EA" "CE>C<" "<B>DG" "<DB>G"
            "<A>CF" "<B>DB"
            "<A>CE8&E16&E32&E64""E2.&E64"
            ","
            // Channel C (ch = 2)
            "T120"
            "$B90"
            "$E1$A0$H100$D100$S90$F2000"
            "V15O4L4"
            "R4"
            "AEA>C<E>C<BDGD<B>G ACF<B>D<BA>CEE2."
            "[2 AEA>C<E>C<BDGD<B>G|ACFFDCG#E<BG#B>E]"
            "ACF<B>D<BA>CEE2."
            "[2 >ECEEC<GBGD<B>DG|AFCFCFG#E<BG#B>E]"
            "AFCE<B>D<A>CEE2."
            "[2 AEA>C<E>C<BDGD<B>G|ACFFDCG#E<BG#B>E]"
            "ACF<B>D<BA>CEE2."
            "$E1$A0$H100$D100$S90$F2000"
            "V14O4L8"
            ">E<EG>CE<G"
            ">E<EG>CE<G"
            "B<B>DGBD"
            "B<DGB>DG"
            "A<A>CFAC"
            "F<FA>DF<A>"
            "G#<G#B>EG#<B>"
            "G#<<B>EG#B>E"
            ">E<EG>CE<G"
            ">E<EG>CE<G"
            "B<B>DGBD"
            "B<DGB>DG"
            "A<A>CFAC"
            "E<EG#B>E<G#"
            ">E<<A>EA>CEE2"
            "$B87V13O5L4"
            "$E1$A20$H100$D100$S70$F600"
            "A>C2DE.$B85F8$B87ED2<BG.A8B"
            ">C.<B8AG#.F#8G#A2.A2."
        },
    },
};

static void mml_callback(uint8_t ch, int32_t param) {
    (void)ch;
    mml_cb_param = param;
    mml_cb_update = true;
}

void Scene001::init(SceneId prev_id) {
    const mgc_sound_psg_if_t *sound_psg = sys_get_sound_driver();
    next_id_ = SceneId::ID_001;
    scene_trans_ = false;
    show_title_ = false;
    show_subtitle_ = false;
    show_main_ = false;
    show_actor_ = false;
    closing_ = false;

    mml_cb_param = 0;
    mml_cb_update = false;

    this->init_components(prev_id);

    cell_mask_ = MGC_CELL_LEN;

    sound_psg->set_bgm_list(mml_records, countof(mml_records));
    sound_psg->set_bgm_callback(mml_callback);
    sound_psg->common_if->play_bgm(0);
}

SceneId Scene001::get_id() const {
    return SceneId::ID_001;
}

SceneId Scene001::get_next_id() const {
    return next_id_;
}

bool Scene001::check_trans() const {
    return scene_trans_;
}

void Scene001::parse_mml_cb_param(int32_t param) {
    if ( param == 1 ) {
        show_title_ = true;
    } else if ( param == 2 ) {
        show_subtitle_ = true;
    } else if ( param == 3 ) {
        show_actor_ = true;
    } else if ( param == 4 ) {
        show_main_ = true;
    } else if ( ( 100 <= param ) && ( param <= 108 ) ) {
        lyrics_index_ = (param-100);
        if ( lyrics_index_ < countof(lyrics_greensleeves) ) {
            dialoguebox_set_text(&dialoguebox_, lyrics_greensleeves[lyrics_index_]);
            dialoguebox_set_enabled(&dialoguebox_, (lyrics_index_ != 0));
        }
    } else {
    }
}

void Scene001::update() {
    int32_t current_param;
    bool update_flag;
    uint32_t status;

    status = sys_save_and_disable_interrupts();
    {
        update_flag = mml_cb_update;
        current_param = mml_cb_param;
        mml_cb_update = false;
    }
    sys_restore_interrupts(status);

    if ( update_flag ) {
        parse_mml_cb_param(current_param);
    }

    dialoguebox_display_update(&dialoguebox_);

    if ( show_title_ ) {
        uint16_t width = sprite_title_.trim_left;
        uint16_t height = sprite_title_.trim_top;
        width  = ( width  >= 8 ) ? ( width  - 8 ) : 0;
        height = ( height >= 8 ) ? ( height - 8 ) : 0;
        sprite_set_trim(&sprite_title_, width, width, height, height);
    }
    if ( show_subtitle_ ) {
        if ( label_subtitle_.width < (8*30) ) {
            label_subtitle_.width += 12;
        }
    }
    if ( show_main_ ) {
        if ( cell_mask_ > 0 ) {
            cell_mask_--;
        }
    }

    if ( show_actor_ ) {
        bool up_flag = false;
        sprite_actor_.x += 1;
        maphit_setup_detection(&maphit_, &sprite_actor_, HITBOX_ACTOR_ID, &tilemap_l1_);
        if ( maphit_detect(&maphit_) == true ) {
            up_flag = true;
        }
        if ( (sprite_actor_.x % 7) <= 3 ) {
            sprite_set_tile_idx(&sprite_actor_, 2);
        } else {
            sprite_set_tile_idx(&sprite_actor_, 1);
        }
        if ( up_flag ) {
            sprite_actor_.y -= 1;
        }

        if ( sprite_actor_.x > MGC_CELL2PIXEL(93) ) {
            sprite_actor_.x = MGC_CELL2PIXEL(93);
            show_main_ = false;
            show_title_ = false;
            show_subtitle_ = false;
            show_actor_ = false;
            closing_ = true;
        }
    }

    if ( closing_ ) {
        if ( cell_mask_ < MGC_CELL_LEN ) {
            cell_mask_++;
        }
    }

    camera_update(&pixelbuffer_, &camera_, &sprite_actor_);
}

void Scene001::draw(uint16_t screen_x0, uint16_t screen_y0, uint16_t width, uint16_t height) {
    const auto display = sys_get_display_driver()->common_if;

    for ( int16_t y = 0; y < height; y += MGC_CELL2PIXEL(1)) {
        for ( int16_t x = 0; x < width; x += MGC_CELL2PIXEL(1)) {
            tilemap_apply_cell_blending(&tilemap_l2_, &pixelbuffer_, x, y);
            tilemap_apply_cell_blending(&tilemap_l1_, &pixelbuffer_, x, y);
            if ( !closing_ ) {
                pixelbuffer_fill_partial_with_color(&pixelbuffer_, MGC_COLOR_BLACK, cell_mask_, MGC_CELL_LEN);
            }
            sprite_apply_cell_blending(&sprite_actor_, &pixelbuffer_, x, y);
            tilemap_apply_cell_blending(&tilemap_l3_, &pixelbuffer_, x, y);
            sprite_apply_cell_blending(&sprite_title_, &pixelbuffer_, x, y);
            label_apply_cell_blending(&label_subtitle_, &pixelbuffer_, x, y);
            dialoguebox_apply_cell_blending(&dialoguebox_, &pixelbuffer_, x, y);
            if ( closing_ ) {
                pixelbuffer_fill_partial_with_color(&pixelbuffer_, MGC_COLOR_BLACK, cell_mask_, MGC_CELL_LEN);
            }
            pixelbuffer_draw_cell(&pixelbuffer_, display, screen_x0+x, screen_y0+y);
        }
    }
}

void Scene001::init_components(SceneId prev_id) {
    // pixelbuffer
    pixelbuffer_init(&pixelbuffer_);
    pixelbuffer_set_back_color(&pixelbuffer_, MGC_COLOR(0.0, 1.0, 1.0));
    pixelbuffer_set_refresh_mode(&pixelbuffer_, true);

    // dialoguebox
    dialoguebox_init(&dialoguebox_, 0, sys_get_default_font(), false);
    dialoguebox_set_position(&dialoguebox_, 8, 8);
    dialoguebox_set_width(&dialoguebox_, 224);
    dialoguebox_set_height(&dialoguebox_, 12*2);
    dialoguebox_set_cursor_speed(&dialoguebox_, 4);
    dialoguebox_set_scroll_speed(&dialoguebox_, 6);
    dialoguebox_set_scroll_line(&dialoguebox_, 2);
    dialoguebox_set_line_spacing(&dialoguebox_, 8);
    dialoguebox_adjust_height(&dialoguebox_);
    dialoguebox_set_text(&dialoguebox_, "");
    dialoguebox_set_r_cell_offset(&dialoguebox_, 0, 0);
    dialoguebox_set_enabled(&dialoguebox_, false);

    // tilemap (layer 1)
    tilemap_init(&tilemap_l1_, 1, &map_001_l1, &tileset_map);
    tilemap_set_position(&tilemap_l1_, MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0));
    tilemap_set_r_cell_offset(&tilemap_l1_, 1, 1);
    tilemap_set_enabled(&tilemap_l1_, true);

    // tilemap (layer 2)
    tilemap_init(&tilemap_l2_, 1, &map_001_l2, &tileset_map);
    tilemap_set_position(&tilemap_l2_, MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(9));
    tilemap_set_r_cell_offset(&tilemap_l2_, 4, 1);
    tilemap_set_enabled(&tilemap_l2_, true);

    // tilemap (layer 3)
    tilemap_init(&tilemap_l3_, 1, &map_001_l3, &tileset_map);
    tilemap_set_position(&tilemap_l3_, MGC_CELL2PIXEL(92), MGC_CELL2PIXEL(0));
    tilemap_set_r_cell_offset(&tilemap_l3_, 1, 1);
    tilemap_set_enabled(&tilemap_l3_, true);

    //  sprite ( Title )
    sprite_init(&sprite_title_, 0);
    sprite_set_tileset(&sprite_title_, &tileset_title_greensleeves);
    sprite_set_tile_idx(&sprite_title_, 0);
    sprite_set_position(&sprite_title_, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(6));
    sprite_set_trim(
        &sprite_title_,
        sprite_title_.tileset->tile_width/2,
        sprite_title_.tileset->tile_width/2,
        sprite_title_.tileset->tile_height/2,
        sprite_title_.tileset->tile_height/2
    );

    // label
    label_init(&label_subtitle_, 0, sys_get_default_font(), false);
    label_set_position(&label_subtitle_, MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(9));
    label_set_size(&label_subtitle_, 0, 12);
    label_set_text(&label_subtitle_, "A Traditional English Folk Song");
    label_set_fore_color(&label_subtitle_, MGC_COLOR(0.4, 0.4, 0.4));
    label_set_enable_back_color(&label_subtitle_, false);

    // Initialize the sprite and hitbox.
    sprite_init(&sprite_actor_, 0);
    sprite_set_tileset(&sprite_actor_, &tileset_actor_greensleeves);
    sprite_set_tile_idx(&sprite_actor_, 1);
    sprite_set_position(&sprite_actor_, MGC_CELL2PIXEL(-1), MGC_CELL2PIXEL(9));
    hitbox_init(&hitbox_actor_, HITBOX_ACTOR_ID, 0, 0, 4, 31);
    sprite_set_hitbox_array(&sprite_actor_, &hitbox_actor_, 1);

    // camera
    camera_init(&camera_);
    camera_set_x_follow_settings(&camera_, MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(83), MGC_CELL2PIXEL(3));
    camera_set_x_follow_enabled(&camera_, true);
    camera_set_y_follow_enabled(&camera_, false);

    // maphit
    maphit_init(&maphit_);
}

}
