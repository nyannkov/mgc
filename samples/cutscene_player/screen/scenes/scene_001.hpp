/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SCENE_001_HPP
#define MGC_SCENE_001_HPP

#include "mgc/mgc.h"
#include "scenes.hpp"

namespace mgc::screen {

struct Scene001 : SceneIf {
    void init(SceneId prev_id) override;
    SceneId get_id() const override;
    SceneId get_next_id() const override;
    bool check_trans() const override;
    void update() override;
    void draw(uint16_t screen_x0, uint16_t screen_y0, uint16_t width, uint16_t height) override;
    ~Scene001() = default;

private:
    SceneId next_id_;
    bool scene_trans_;
    bool show_title_;
    bool show_subtitle_;
    bool show_main_;
    bool show_actor_;
    bool closing_;
    size_t lyrics_index_;
    mgc_pixelbuffer pixelbuffer_;
    mgc_camera camera_;
    mgc_dialoguebox dialoguebox_;
    mgc_tilemap tilemap_l1_;
    mgc_tilemap tilemap_l2_;
    mgc_tilemap tilemap_l3_;
    mgc_label label_subtitle_;
    mgc_sprite sprite_title_;
    mgc_sprite sprite_actor_;
    mgc_hitbox hitbox_actor_;
    mgc_maphit maphit_;
    uint16_t cell_mask_;
    void init_components(SceneId prev_id);
    void parse_mml_cb_param(int32_t param);
};

}
#endif/*MGC_SCENE_001_HPP*/
