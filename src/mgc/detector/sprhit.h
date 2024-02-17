/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SPRHIT_H
#define MGC_SPRHIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/components/sprite.h"

enum mgc_sprhit_state {
    MGC_SPRHIT_STATE_INIT,
    MGC_SPRHIT_STATE_READY,
    MGC_SPRHIT_STATE_IN_PROGRESS,
    MGC_SPRHIT_STATE_END,
    MGC_SPRHIT_STATE_ERROR
};

typedef struct mgc_sprhit {
    mgc_id_t hit_opp_hitbox_id;
    enum mgc_sprhit_state state;
    int16_t l0;
    int16_t r0;
    int16_t t0;
    int16_t b0;
    const mgc_sprite_t *opponent;
    size_t opp_hitbox_idx;
} mgc_sprhit_t;

#define SPRHIT_GET_HIT_OPP_HITBOX_ID(sprhit) ((sprhit).hit_opp_hitbox_id)

extern void sprhit_init(mgc_sprhit_t *sprhit);
extern void sprhit_setup_detection(mgc_sprhit_t *sprhit, const mgc_sprite_t *target, mgc_id_t target_hitbox_id, const mgc_sprite_t *opponent);
extern bool sprhit_detect(mgc_sprhit_t *sprhit);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SPRHIT_H*/
