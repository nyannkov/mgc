/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SUB_SCREEN_HPP
#define MGC_SUB_SCREEN_HPP

#include "scenes/scene_sub.hpp"

namespace mgc::sub_screen {

struct Screen {
    Screen(int16_t x, int16_t y, uint8_t rows, uint8_t cols) {
        x_ = x;
        y_ = y;
        rows_ = rows;
        cols_ = cols;
    }
    void init() {
        scene_.init();
    }
    void update() {
        scene_.update();
        scene_.draw(x_, y_, MGC_CELL2PIXEL(rows_), MGC_CELL2PIXEL(cols_));
    }

private:
    int16_t x_;
    int16_t y_;
    uint8_t rows_;
    uint8_t cols_;
    SceneSub scene_;
};
}
#endif/*MGC_SUB_SCREEN_HPP*/
