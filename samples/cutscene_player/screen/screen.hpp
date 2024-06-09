/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SCREEN_HPP
#define MGC_SCREEN_HPP

#include "scenes/scenes.hpp"

namespace mgc::screen {

struct Screen {
    Screen(int16_t x, int16_t y, uint8_t rows, uint8_t cols) {
        x_ = x;
        y_ = y;
        rows_ = rows;
        cols_ = cols;
        scene_ = std::move(make_scene_by_id(SceneId::ID_001));
    }
    void init(SceneId prev_id) {
        scene_->init(prev_id);
    }
    void update() {
        if (!scene_) {
            MGC_WARN("scene is nullptr");
            return;
        }
        scene_->update();
        scene_->draw(x_, y_, MGC_CELL2PIXEL(rows_), MGC_CELL2PIXEL(cols_));
        if ( scene_->check_trans() ) {
            SceneId prev_id = scene_->get_id();
            SceneId next_id = scene_->get_next_id();
            scene_.reset();
            scene_ = make_scene_by_id(next_id);
            if ( scene_ ) {
                scene_->init(prev_id);
            }
        }
    }

private:
    int16_t x_;
    int16_t y_;
    uint8_t rows_;
    uint8_t cols_;
    std::unique_ptr<SceneIf>  scene_;
};

}
#endif/*MGC_SCREEN_HPP*/
