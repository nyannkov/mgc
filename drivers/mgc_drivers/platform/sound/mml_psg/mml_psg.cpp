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

typedef int64_t           q_t;

#define Q_VALUE           (16)
#define Q_TO_F(q)         (float)((float)(q) / (1 << Q_VALUE))
#define F_TO_Q(f)         (q_t)((f) * (1 << Q_VALUE) + 0.5f)
#define MUL_Q(q1, q2)     (((q_t)(q1) * (q_t)(q2)) >> Q_VALUE)

#ifndef MGC_MML_PSG_DEFAULT_LPF_ALPHA
#define MGC_MML_PSG_DEFAULT_LPF_ALPHA           F_TO_Q(0.4f)
#endif/*MGC_MML_PSG_DEFAULT_LPF_ALPHA*/


#ifndef MGC_MML_PSG_LPF_SHIFT_BIT
#define MGC_MML_PSG_LPF_SHIFT_BIT               (3)
#endif/*MGC_MML_PSG_LPF_SHIFT_BIT*/

#ifndef MGC_MML_PSG_EMULATOR_CLOCK
#define MGC_MML_PSG_EMULATOR_CLOCK              (2000000.0f)
#endif/*MGC_MML_PSG_EMULATOR_CLOCK*/

#ifndef MGC_MML_PSG_EMULATOR_RATE
#define MGC_MML_PSG_EMULATOR_RATE               (40000.0f)
#endif/*MGC_MML_PSG_EMULATOR_RATE*/

#ifndef MGC_MML_PSG_MML_PROC_RATE
#define MGC_MML_PSG_MML_PROC_RATE               (400.0f)
#endif/*MGC_MML_PSG_MML_PROC_RATE*/

#define EMULATOR_CLOCK          MGC_MML_PSG_EMULATOR_CLOCK
#define EMULATOR_RATE           MGC_MML_PSG_EMULATOR_RATE
#define MML_PROC_RATE           MGC_MML_PSG_MML_PROC_RATE
#define LPF_DEFAULT_ALPHA       MGC_MML_PSG_DEFAULT_LPF_ALPHA
#define LPF_SHIFT_BIT           MGC_MML_PSG_LPF_SHIFT_BIT


typedef struct mgc_mml_record_list {
    const mgc_mml_record_t *records;
    size_t record_count;
} mgc_mml_record_list_t;

static PsginoZ psgino_z;
static PSG* psg;

static q_t psg_lpf_alpha_q;
static q_t psg_output_1_q;
static q_t psg_output_2_q;

static bool psg_lpf_enabled;
static mgc_mml_record_list_t bgm_list, se_list;
static void* context;
static void (*cb_background_music)(uint8_t ch, int32_t param, void *ctx);
static void (*cb_sound_effect)(uint8_t ch, int32_t param, void *ctx);
static q_t master_volume_q;


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

    master_volume_q = F_TO_Q(1.0f);
    psg_lpf_enabled = true;
    psg_lpf_alpha_q = LPF_DEFAULT_ALPHA;
    psg_output_1_q = F_TO_Q(0.0f);
    psg_output_2_q = F_TO_Q(0.0f);

    psg = PSG_new(EMULATOR_CLOCK, EMULATOR_RATE);
    PSG_reset(psg);
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
    if ( volume < 0.0F ) {
        master_volume_q = F_TO_Q(0.0f);
    } else if ( volume > 1.0F ) {
        master_volume_q = F_TO_Q(1.0f);
    } else {
        master_volume_q = F_TO_Q(volume);
    }
}

void mml_psg_set_background_music_volume(float volume) {
    // NOTE: Not supported.
}

void mml_psg_set_sound_effect_volume(float volume) {
    // NOTE: Not supported.
}

float mml_psg_get_master_volume() {
    return Q_TO_F(master_volume_q);
}

float mml_psg_get_background_music_volume() {
    return Q_TO_F(master_volume_q);
}

float mml_psg_get_sound_effect_volume() {
    return Q_TO_F(master_volume_q);
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
    if ( alpha < 0.0f ) {
        psg_lpf_alpha_q = F_TO_Q(0.0f);

    } else if ( alpha > 1.0f ) {
        psg_lpf_alpha_q = F_TO_Q(1.0f);

    } else {
        psg_lpf_alpha_q = F_TO_Q(alpha);
    }
}

void mml_psg_set_speed_factor(float factor) {
    uint16_t factor_pct = 0;
    if ( factor < 0.0f ) {
        factor_pct = 0;
    } else {
        factor_pct = static_cast<uint16_t>(factor * 100 + 0.5f);
    }
    psgino_z.SetSpeedFactor(factor_pct);
}

void mml_psg_shift_pitch_by_degree(int16_t degree) {
    psgino_z.ShiftFrequency(degree);
}

uint16_t mml_psg_local__proc_psg_emu(void) {

    q_t output_q = 0;
    if( psg != nullptr ) {
        q_t input_q = 0;
        input_q = (q_t)(static_cast<int32_t>(PSG_calc(psg)) << Q_VALUE);
        input_q >>= LPF_SHIFT_BIT;
        if ( psg_lpf_enabled ) {
            q_t delta_1 = input_q - psg_output_1_q;
            psg_output_1_q += MUL_Q(delta_1, psg_lpf_alpha_q);

            q_t delta_2 = psg_output_1_q - psg_output_2_q;
            psg_output_2_q += MUL_Q(delta_2, psg_lpf_alpha_q);

            output_q = psg_output_2_q;
        } else {
            output_q = input_q;
        }
    } else {
        output_q = 0;
    }

    output_q = MUL_Q(output_q, master_volume_q);

    return static_cast<uint16_t>(output_q >> Q_VALUE);
}

void mml_psg_local__proc_psg_mml(void) {

    psgino_z.Proc();
}

