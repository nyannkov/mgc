/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "sprhit.h"

void sprhit_init(mgc_sprhit_t *sprhit) {
    if ( sprhit == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprhit->hit_opp_hitbox_id = 0;
    sprhit->state = MGC_SPRHIT_STATE_INIT;
}

void sprhit_setup_detection(mgc_sprhit_t *sprhit, const mgc_sprite_t *target, mgc_id_t target_hitbox_id, const mgc_sprite_t *opponent) {
    bool hitbox_id_found = false;
    const mgc_hitbox_t *hitbox;
    if ( sprhit == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    if ( ( target == NULL ) ||
         ( target->hitbox_array == NULL ) ||
         ( target->hitbox_count == 0 ) ||
         ( opponent == NULL ) ||
         ( opponent->hitbox_array == NULL ) ||
         ( opponent->hitbox_count == 0 )
    ) {
        MGC_WARN("Invalid handler");
        sprhit->state = MGC_SPRHIT_STATE_ERROR;
        return;
    }
    if ( ( target->enabled == false ) ||
         ( opponent->enabled == false )
    ) {
        MGC_INFO("Handler is disabled");
        sprhit->state = MGC_SPRHIT_STATE_END;
        return;
    }

    for ( size_t idx = 0; idx < target->hitbox_count; idx++ ) {
        hitbox = &target->hitbox_array[idx];
        if ( hitbox->id == target_hitbox_id ) {
            hitbox_id_found = true;
            break;
        }
    }
    if ( hitbox_id_found == false ) {
        MGC_WARN("target_hitbox_id is not found");
        sprhit->state = MGC_SPRHIT_STATE_ERROR;
        return;
    }

    sprhit->l0 = target->x + hitbox->x0_ofs;
    sprhit->r0 = target->x + hitbox->x0_ofs + hitbox->width - 1;
    sprhit->t0 = target->y + hitbox->y0_ofs;
    sprhit->b0 = target->y + hitbox->y0_ofs + hitbox->height - 1;
    sprhit->opponent = opponent;
    sprhit->opp_hitbox_idx = 0;
    sprhit->state = MGC_SPRHIT_STATE_READY;
}

bool sprhit_detect(mgc_sprhit_t *sprhit) {
    const mgc_sprite_t *opponent;
    if ( sprhit == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( ( sprhit->state == MGC_SPRHIT_STATE_INIT ) ||
         ( sprhit->state == MGC_SPRHIT_STATE_ERROR ) ||
         ( sprhit->state == MGC_SPRHIT_STATE_END )
    ) {
        return false;
    }

    opponent = sprhit->opponent;

    for ( ; sprhit->opp_hitbox_idx < opponent->hitbox_count; sprhit->opp_hitbox_idx++ ) {
        int16_t l1, r1, t1, b1;
        const mgc_hitbox_t *hitbox = &opponent->hitbox_array[sprhit->opp_hitbox_idx];
        l1 = opponent->x + hitbox->x0_ofs;
        r1 = opponent->x + hitbox->x0_ofs + hitbox->width - 1;
        t1 = opponent->y + hitbox->y0_ofs;
        b1 = opponent->y + hitbox->y0_ofs + hitbox->height - 1;
        if ( (sprhit->l0<r1) && (l1<sprhit->r0) && (sprhit->t0<b1) && (t1<sprhit->b0) ) {
            sprhit->state = MGC_SPRHIT_STATE_IN_PROGRESS;
            sprhit->hit_opp_hitbox_id = hitbox->id;
            sprhit->opp_hitbox_idx++;
            return true;
        }
    }
    sprhit->state = MGC_SPRHIT_STATE_END;
    return false;
}
