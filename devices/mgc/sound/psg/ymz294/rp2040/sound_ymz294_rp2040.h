/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SOUND_YMZ294_RP2040_H
#define MGC_SOUND_YMZ294_RP2040_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/sound/interface/sound_if.h"

typedef struct mml_sound_record {
    mgc_sound_id_t id;
    const char *mml;
} mml_sound_record_t;

extern const struct mgc_sound_if sound_driver_ymz294_rp2040;

void sound_driver_ymz294_rp2040_set_bgm_table(const mml_sound_record_t *records, size_t count);
void sound_driver_ymz294_rp2040_set_se_table(const mml_sound_record_t *records, size_t count);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SOUND_YMZ294_RP2040_H*/
