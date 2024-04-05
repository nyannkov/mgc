/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SOUND_PSG_YMZ294_H
#define MGC_SOUND_PSG_YMZ294_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/sound/interface/sound_if.h"
#include "mgc/sound/psg/common/mml_record.h"

const mgc_sound_if_t *sound_psg_ymz294_get_instance(void);
void sound_psg_ymz294_set_bgm_list(const mgc_mml_record_t *records, size_t count);
void sound_psg_ymz294_set_se_list(const mgc_mml_record_t *records, size_t count);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SOUND_PSG_YMZ294_H*/
