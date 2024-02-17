/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SCENE_002_HPP
#define MGC_SCENE_002_HPP

#include "mgc/mgc.h"
#include "scenes.hpp"
#include "actor/player.hpp"
#include "actor/static_object.hpp"

namespace mgc::main_screen {

struct Scene002 : SceneIf {
    void init(SceneId prev_id) override;
    SceneId get_id() const override;
    SceneId get_next_id() const override;
    bool check_trans() const override;
    void update() override;
    void draw(uint16_t screen_x0, uint16_t screen_y0, uint16_t width, uint16_t height) override;
    ~Scene002() = default;

private:
    SceneId next_id_;
    bool scene_trans_;
    Player player_;
    StaticObject gate_;
    StaticObject signboard_;
    mgc_pixelbuffer pixelbuffer_;
    mgc_camera camera_;
    mgc_tilemap tilemap_;
    mgc_tilemap tilemap_bg_;
    mgc_maphit maphit_;
    mgc_sprhit sprhit_;
    mgc_dlgctrl dlgctrl_;
    SceneState scene_state_;
    int16_t cell_mask_;
    void init_components(SceneId prev_id);
};

}
#endif/*MGC_SCENE_002_HPP*/
