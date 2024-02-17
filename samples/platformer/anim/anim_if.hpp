/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_ANIM_IF_HPP
#define MGC_ANIM_IF_HPP
#include "mgc/mgc.h"

namespace mgc {

enum class AnimType {
    Invisible,
    Stop,
    Walk,
    Crouch,
    Rising,
    Falling,
};

enum class AnimDir {
    Left,
    Right,
};

struct AnimIf {
    virtual const mgc_tileset_t *get_ptr_tileset() const = 0;
    virtual size_t get_tile_idx() const = 0;
    virtual void set_anim_type(AnimType type) = 0;
    virtual void set_anim_dir(AnimDir dir) = 0;
    virtual void update() = 0;
};

}
#endif/*MGC_ANIM_IF_HPP*/
