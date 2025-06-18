/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_MML_PSG_H
#define MGC_DRIVERS_MML_PSG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

typedef struct mgc_mml_record {
    const int id;
    const char *mml;
} mgc_mml_record_t;

void mml_psg_init(float mml_proc_rate, void *ctx);
void mml_psg_deinit(void);

bool mml_psg_play_background_music(int music_id, float fade_in_sec);
bool mml_psg_play_sound_effect(int effect_id, float fade_in_sec);

void mml_psg_stop_background_music(float fade_out_in_sec);
void mml_psg_stop_all_sound_effects(float fade_out_in_sec);

bool mml_psg_is_background_music_playing(void);
bool mml_psg_is_any_sound_effect_playing(void);

bool mml_psg_has_background_music_finished(void);
bool mml_psg_have_all_sound_effects_finished(void);

void mml_psg_set_master_volume(float volume);
void mml_psg_set_background_music_volume(float volume);
void mml_psg_set_sound_effect_volume(float volume);

float mml_psg_get_master_volume();
float mml_psg_get_background_music_volume();
float mml_psg_get_sound_effect_volume();

void mml_psg_set_psg_lpf_enabled(bool enabled);
void mml_psg_set_psg_lpf_alpha(float alpha);
void mml_psg_set_background_music_list(const mgc_mml_record_t *records, size_t count);
void mml_psg_set_sound_effect_list(const  mgc_mml_record_t *records, size_t count);
void mml_psg_set_callback_background_music(void (*cb)(uint8_t ch, int32_t param, void * ctx));
void mml_psg_set_callback_sound_effect(void (*cb)(uint8_t ch, int32_t param, void * ctx));

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_MML_PSG_H*/
