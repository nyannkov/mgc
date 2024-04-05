/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SOUND_PSG_H
#define MGC_SOUND_PSG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/sound/interface/sound_if.h"
#include "mgc/sound/psg/common/mml_record.h"

extern const mgc_sound_if_t * ( * const sound_psg_get_instance )(void);
extern void ( * const sound_psg_set_bgm_list)(const mgc_mml_record_t *records, size_t count);
extern void ( * const sound_psg_set_se_list)(const mgc_mml_record_t *records, size_t count);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SOUND_PSG_H*/
