/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "mml_psg.h"
#include "mml_psg_local.h"
#include "ports/mml_psg_port.h"
#include "emu2149.h"
#include "Psgino.h"

typedef struct mgc_mml_record_list {
    const mgc_mml_record_t *records;
    size_t record_count;
} mgc_mml_record_list_t;

static PsginoZ psgino_z;
static PSG* psg;
static float prev_psg_output;
static float psg_lpf_alpha;
static bool psg_lpf_enabled;
static mgc_mml_record_list_t bgm_list, se_list;
static void* context;
static void (*cb_background_music)(uint8_t ch, int32_t param, void *ctx);
static void (*cb_sound_effect)(uint8_t ch, int32_t param, void *ctx);
static float master_volume;

#ifndef MGC_MML_PSG_RP2040_DEFAULT_LPF_ALPHA
#define MGC_MML_PSG_RP2040_DEFAULT_LPF_ALPHA           (0.7F)
#endif/*MGC_MML_PSG_RP2040_DEFAULT_LPF_ALPHA*/

#ifndef MGC_MML_PSG_RP2040_EMULATOR_CLOCK
#define MGC_MML_PSG_RP2040_EMULATOR_CLOCK              (2000000.0F)
#endif/*MGC_MML_PSG_RP2040_EMULATOR_CLOCK*/

#ifndef MGC_MML_PSG_RP2040_EMULATOR_RATE
#define MGC_MML_PSG_RP2040_EMULATOR_RATE               (40000.0F)
#endif/*MGC_MML_PSG_RP2040_EMULATOR_RATE*/

#ifndef MGC_MML_PSG_RP2040_MML_PROC_RATE
#define MGC_MML_PSG_RP2040_MML_PROC_RATE               (400.0F)
#endif/*MGC_MML_PSG_RP2040_MML_PROC_RATE*/

#define EMULATOR_CLOCK      MGC_MML_PSG_RP2040_EMULATOR_CLOCK
#define EMULATOR_RATE       MGC_MML_PSG_RP2040_EMULATOR_RATE
#define MML_PROC_RATE       MGC_MML_PSG_RP2040_MML_PROC_RATE
#define LPF_DEFAULT_ALPHA   MGC_MML_PSG_RP2040_DEFAULT_LPF_ALPHA

static void psg_write(uint8_t addr, uint8_t data) {

    mml_psg_port__psg_guard_enter();

    PSG_writeReg(psg, addr, data);

    mml_psg_port__psg_guard_exit();
}

static void callback_wrapper_background_music(uint8_t ch, int32_t param) {
    if ( cb_background_music != nullptr ) {
        cb_background_music(ch, param, context);
    }
}

static void callback_wrapper_sound_effect(uint8_t ch, int32_t param) {
    if ( cb_sound_effect != nullptr ) {
        cb_sound_effect(ch, param, context);
    }
}

void mml_psg_init(float mml_proc_rate, void *ctx) {
    if ( psg != nullptr ) {
        return;
    }
    float proc_rate;
    if ( mml_proc_rate <= 0.0F ) {
        mml_proc_rate = MML_PROC_RATE;
    }

    master_volume = 1.0F;
    psg_lpf_alpha = LPF_DEFAULT_ALPHA;
    prev_psg_output = 0.0F;
    psg = PSG_new(EMULATOR_CLOCK, EMULATOR_RATE);
    psgino_z.Initialize(psg_write, EMULATOR_CLOCK, mml_proc_rate);

    mml_psg_port__init(EMULATOR_RATE, mml_proc_rate);

    context = ctx;
    cb_background_music = nullptr;
    cb_sound_effect = nullptr;
    psgino_z.SetUserCallback(callback_wrapper_background_music);
    psgino_z.SetSeUserCallback(callback_wrapper_sound_effect);
}

void mml_psg_deinit(void) {

    mml_psg_port__deinit();

    if ( psg != nullptr ) {
        PSG_delete(psg);
        psg = nullptr;
    }
}

bool mml_psg_play_background_music(int music_id, float fade_in_sec) {
    // NOTE: Fade-in and fade-out are not supported.
    (void)fade_in_sec;

    for ( size_t i = 0; i < bgm_list.record_count; i++ ) {
        const mgc_mml_record_t *record = &bgm_list.records[i];
        if ( record->id == music_id ) {
            psgino_z.SetMML(record->mml);
            psgino_z.Play();
            return true;
        }
    }
    return false;
}

bool mml_psg_play_sound_effect(int effect_id, float fade_in_sec) {
    // NOTE: Fade-in and fade-out are not supported.
    (void)fade_in_sec;

    for ( size_t i = 0; i < se_list.record_count; i++ ) {
        const mgc_mml_record *record = &se_list.records[i];
        if ( record->id == effect_id ) {
            psgino_z.SetSeMML(record->mml);
            psgino_z.PlaySe();
            return true;
        }
    }
    return false;
}

void mml_psg_stop_background_music(float fade_out_in_sec) {
    // NOTE: Fade-in and fade-out are not supported.
    (void)fade_out_in_sec;

    psgino_z.Stop();
}

void mml_psg_stop_all_sound_effects(float fade_out_in_sec) {
    // NOTE: Fade-in and fade-out are not supported.
    (void)fade_out_in_sec;

    psgino_z.StopSe();
}

bool mml_psg_is_background_music_playing(void) {
    return psgino_z.GetStatus() == Psgino::Playing;
}

bool mml_psg_is_any_sound_effect_playing(void) {
    // NOTE: Only one sound effect can be played at a time. 
    return psgino_z.GetSeStatus() == Psgino::Playing;
}

bool mml_psg_has_background_music_finished(void) {
    return psgino_z.GetStatus() == Psgino::PlayEnd;
}

bool mml_psg_have_all_sound_effects_finished(void) {
    // NOTE: Only one sound effect can be played at a time. 
    return psgino_z.GetSeStatus() == Psgino::PlayEnd;
}

void mml_psg_set_master_volume(float volume) {
    // NOTE: Not supported.
}

void mml_psg_set_background_music_volume(float volume) {
    // NOTE: Not supported.
}

void mml_psg_set_sound_effect_volume(float volume) {
    if ( volume < 0.0F ) {
        master_volume = 0.0F;
    } else if ( volume > 1.0F ) {
        master_volume = 1.0F;
    } else {
        master_volume = volume;
    }
}

float mml_psg_get_master_volume() {
    return master_volume;
}

float mml_psg_get_background_music_volume() {
    return master_volume;
}

float mml_psg_get_sound_effect_volume() {
    return master_volume;
}

void mml_psg_set_background_music_list(const mgc_mml_record_t *records, size_t count) {
    bgm_list.records = records;
    bgm_list.record_count = count;
}

void mml_psg_set_sound_effect_list(const  mgc_mml_record_t *records, size_t count) {
    se_list.records = records;
    se_list.record_count = count;
}

void mml_psg_set_callback_background_music(void (*cb)(uint8_t ch, int32_t param, void * ctx)) {

    cb_background_music = cb;
}

void mml_psg_set_callback_sound_effect(void (*cb)(uint8_t ch, int32_t param, void * ctx)) {

    cb_sound_effect = cb;
}

void mml_psg_set_psg_lpf_enabled(bool enabled) {
    psg_lpf_enabled = enabled;
}

void mml_psg_set_psg_lpf_alpha(float alpha) {
    if ( alpha < 0.0F ) {
        psg_lpf_alpha = 0.0F;
    } else if ( alpha > 1.0F ) {
        psg_lpf_alpha = 1.0F;
    } else {
        psg_lpf_alpha = alpha;
    }
}

void mml_psg_set_speed_factor(float factor) {
    psgino_z.SetSpeedFactor(factor);
}

void mml_psg_shift_pitch_by_degree(int16_t degree) {
    psgino_z.ShiftFrequency(degree);
}

uint16_t mml_psg_local__proc_psg_emu(void) {

    if( psg != nullptr ) {
        float psg_output = (float)PSG_calc(psg);
        if ( psg_lpf_enabled ) {
            prev_psg_output = 
                prev_psg_output + psg_lpf_alpha * ( psg_output - prev_psg_output );
        } else {
            prev_psg_output = psg_output;
        }
    } else {
        prev_psg_output = 0.0F;
    }
    prev_psg_output *= master_volume;

    return (uint16_t)(prev_psg_output + 0.5F)>>8;
}

void mml_psg_local__proc_psg_mml(void) {

    psgino_z.Proc();
}

