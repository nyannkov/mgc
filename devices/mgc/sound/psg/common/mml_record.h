/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_MML_RECORD_H
#define MGC_MML_RECORD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/sound/interface/sound_if.h"

typedef struct mgc_mml_record {
    mgc_sound_id_t id;
    const char *mml;
} mgc_mml_record_t;

typedef struct mgc_mml_list {
    const mgc_mml_record_t *records;
    size_t record_count;
} mgc_mml_list_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_MML_RECORD_H*/

