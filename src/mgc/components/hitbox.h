/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_HITBOX_H
#define MGC_HITBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef struct mgc_hitbox {
    mgc_id_t id;
    int8_t x0_ofs;
    int8_t y0_ofs;
    uint16_t width;
    uint16_t height;
    bool enabled;
} mgc_hitbox_t;

void hitbox_init(mgc_hitbox_t *hitbox, mgc_id_t id, int8_t x0_ofs, int8_t y0_ofs, uint16_t width, uint16_t height);
void hitbox_set_enabled(mgc_hitbox_t *hitbox, bool enabled);
bool hitbox_is_enabled(const mgc_hitbox_t *hitbox);
const mgc_hitbox_t *hitbox_get_by_id(const mgc_hitbox_t *hitbox_array, size_t hitbox_count, mgc_id_t id);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_HITBOX_H*/
