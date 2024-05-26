/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SCENES_HPP
#define MGC_SCENES_HPP

#include <memory>
#include "mgc/mgc.h"

namespace mgc::screen {

enum class SceneId : uint16_t {
    ID_000 = 0,
    ID_001,
};

enum class SceneState {
    Closing,
    Closed,
    Showing,
    Shown,
};

struct SceneIf {
    virtual void init(SceneId prev_id) = 0;
    virtual SceneId get_id() const = 0;
    virtual SceneId get_next_id() const = 0;
    virtual bool check_trans() const = 0;
    virtual void update() = 0;
    virtual void draw(uint16_t screen_x0, uint16_t screen_y0, uint16_t width, uint16_t height) = 0;
    virtual ~SceneIf() = default;
};

std::unique_ptr<SceneIf> make_scene_by_id(SceneId id);

}
#endif/*MGC_SCENES_HPP*/
