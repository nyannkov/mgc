/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_STATIC_OBJECT_HPP
#define MGC_STATIC_OBJECT_HPP

#include "mgc/mgc.h"
#include "actor_if.hpp"

namespace mgc {

struct StaticObject : ActorIf {
    void init(mgc_id_t id, int16_t x, int16_t y) override;
    const mgc_sprite *get_ptr_sprite() const override;
    void set_tileset(const mgc_tileset &tileset);
    void set_tile_idx(size_t idx);
    void init_hitbox(mgc_id_t id, int8_t x0_ofs, int8_t y0_ofs, uint8_t width, uint8_t height);
private:
    mgc_hitbox hitbox_;
    mgc_sprite sprite_;
};

}
#endif/*MGC_STATIC_OBJECT_HPP*/
