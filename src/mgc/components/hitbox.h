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
    uint8_t width;
    uint8_t height;
} mgc_hitbox_t;

void hitbox_init(mgc_hitbox_t *hitbox, mgc_id_t id, int8_t x0_ofs, int8_t y0_ofs, uint8_t width, uint8_t height);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_HITBOX_H*/
