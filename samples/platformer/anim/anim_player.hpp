/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_ANIM_PLAYER_HPP
#define MGC_ANIM_PLAYER_HPP

#include "anim_if.hpp"

namespace mgc {

struct AnimPlayer : AnimIf {
    AnimPlayer();
    const mgc_tileset_t *get_ptr_tileset() const override;
    size_t get_tile_idx() const override;
    void set_anim_type(AnimType type) override;
    void set_anim_dir(AnimDir dir) override;
    void update() override;
private:
    AnimType anim_type_;
    AnimDir anim_dir_;
    size_t tile_idx_;
};

}
#endif/*MGC_ANIM_PLAYER_HPP*/
