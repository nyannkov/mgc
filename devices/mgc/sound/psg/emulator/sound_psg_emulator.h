
/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SOUND_PSG_EMULATOR_H
#define MGC_SOUND_PSG_EMULATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/sound/psg/common/sound_psg_if.h"

const mgc_sound_psg_if_t *sound_psg_emulator_get_instance(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SOUND_PSG_EMULATOR_H*/
