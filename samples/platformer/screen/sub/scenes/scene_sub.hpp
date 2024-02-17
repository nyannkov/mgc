/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SCENE_SUB_HPP
#define MGC_SCENE_SUB_HPP

#include "mgc/mgc.h"

namespace mgc::sub_screen {

struct SceneSub {
    void init();
    void update();
    void draw(uint16_t screen_x0, uint16_t screen_y0, uint16_t width, uint16_t height);
    ~SceneSub() = default;
private:
    mgc_pixelbuffer pixelbuffer_;
    mgc_dialoguebox dialoguebox_;
    mgc_selectbox yesnobox_;
    mgc_label label_frame_rate_;
    mgc_sprite sprte_life_;
    char str_frame_rate_[32];
    bool all_draw_flag_;
    void init_components();
    void update_frame_rate();
};

}
#endif/*MGC_SCENE_SUB_HPP*/
