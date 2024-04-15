/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SOUND_PSG_IF_H
#define MGC_SOUND_PSG_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/sound/interface/sound_if.h"
#include "mml_record.h"

typedef struct mgc_sound_psg_if {
    const mgc_sound_if_t * common_if;
    void (*set_bgm_list)(const mgc_mml_record_t *records, size_t count);
    void (*set_se_list)(const mgc_mml_record_t *records, size_t count);
    void (*set_bgm_callback)(void (*cb)(uint8_t ch, int32_t param));
    void (*set_se_callback)(void (*cb)(uint8_t ch, int32_t param));
    void (*finish_primary_loop)(void);
} mgc_sound_psg_if_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SOUND_PSG_IF_H*/
