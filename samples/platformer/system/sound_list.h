/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SOUND_LIST_H
#define MGC_SOUND_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

enum bgm_id {
    BGM_ID_000 = 0,
    NUM_OF_BGM
};

enum se_id {
    SE_ID_000 = 0,
    SE_ID_001,
    SE_ID_002,
    NUM_OF_SE
};

void init_sound_lst(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SOUND_LIST_H*/
