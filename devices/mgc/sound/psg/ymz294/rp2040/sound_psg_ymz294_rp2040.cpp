/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/irq.h>
#include "mgc/sound/psg/common/Psgino/src/Psgino.h"
#include "mgc/sound/psg/ymz294/sound_psg_ymz294.h"

#ifndef MGC_PIN_YMZ294_PHI_M
#define MGC_PIN_YMZ294_PHI_M        (13)
#endif/*MGC_PIN_YMZ294_PHI_M*/

#ifndef MGC_PIN_YMZ294_AO
#define MGC_PIN_YMZ294_AO           (14)
#endif/*MGC_PIN_YMZ294_AO*/

#ifndef MGC_PIN_YMZ294_WRCS_N
#define MGC_PIN_YMZ294_WRCS_N       (15)
#endif/*MGC_PIN_YMZ294_WRCS_N*/

#ifndef MGC_PIN_YMZ294_D0
#define MGC_PIN_YMZ294_D0           (4)
#endif/*MGC_PIN_YMZ294_D0*/

#ifndef MGC_PIN_YMZ294_D1
#define MGC_PIN_YMZ294_D1           (3)
#endif/*MGC_PIN_YMZ294_D1*/

#ifndef MGC_PIN_YMZ294_D2
#define MGC_PIN_YMZ294_D2           (2)
#endif/*MGC_PIN_YMZ294_D2*/

#ifndef MGC_PIN_YMZ294_D3
#define MGC_PIN_YMZ294_D3           (22)
#endif/*MGC_PIN_YMZ294_D3*/

#ifndef MGC_PIN_YMZ294_D4
#define MGC_PIN_YMZ294_D4           (21)
#endif/*MGC_PIN_YMZ294_D4*/

#ifndef MGC_PIN_YMZ294_D5
#define MGC_PIN_YMZ294_D5           (18)
#endif/*MGC_PIN_YMZ294_D5*/

#ifndef MGC_PIN_YMZ294_D6
#define MGC_PIN_YMZ294_D6           (19)
#endif/*MGC_PIN_YMZ294_D6*/

#ifndef MGC_PIN_YMZ294_D7
#define MGC_PIN_YMZ294_D7           (20)
#endif/*MGC_PIN_YMZ294_D7*/

#define PIN_YMZ294_PHI_M            MGC_PIN_YMZ294_PHI_M
#define PIN_YMZ294_AO               MGC_PIN_YMZ294_AO
#define PIN_YMZ294_WRCS_N           MGC_PIN_YMZ294_WRCS_N
#define PIN_YMZ294_D0               MGC_PIN_YMZ294_D0
#define PIN_YMZ294_D1               MGC_PIN_YMZ294_D1
#define PIN_YMZ294_D2               MGC_PIN_YMZ294_D2
#define PIN_YMZ294_D3               MGC_PIN_YMZ294_D3
#define PIN_YMZ294_D4               MGC_PIN_YMZ294_D4
#define PIN_YMZ294_D5               MGC_PIN_YMZ294_D5
#define PIN_YMZ294_D6               MGC_PIN_YMZ294_D6
#define PIN_YMZ294_D7               MGC_PIN_YMZ294_D7

PsginoZ psgino_z;
static struct repeating_timer timer;
static mgc_mml_list_t bgm_list, se_list;

static void psg_init(void) {
    gpio_init(PIN_YMZ294_AO);
    gpio_init(PIN_YMZ294_WRCS_N);
    gpio_init(PIN_YMZ294_D0);
    gpio_init(PIN_YMZ294_D1);
    gpio_init(PIN_YMZ294_D2);
    gpio_init(PIN_YMZ294_D3);
    gpio_init(PIN_YMZ294_D4);
    gpio_init(PIN_YMZ294_D5);
    gpio_init(PIN_YMZ294_D6);
    gpio_init(PIN_YMZ294_D7);

    gpio_set_dir(PIN_YMZ294_AO, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_WRCS_N, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D0, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D1, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D2, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D3, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D4, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D5, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D6, GPIO_OUT);
    gpio_set_dir(PIN_YMZ294_D7, GPIO_OUT);

    gpio_put(PIN_YMZ294_AO, 0);
    gpio_put(PIN_YMZ294_WRCS_N, 1);
    gpio_put(PIN_YMZ294_D0, 0);
    gpio_put(PIN_YMZ294_D1, 0);
    gpio_put(PIN_YMZ294_D2, 0);
    gpio_put(PIN_YMZ294_D3, 0);
    gpio_put(PIN_YMZ294_D4, 0);
    gpio_put(PIN_YMZ294_D5, 0);
    gpio_put(PIN_YMZ294_D6, 0);
    gpio_put(PIN_YMZ294_D7, 0);

    /* phiM */
    gpio_set_function(PIN_YMZ294_PHI_M, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PIN_YMZ294_PHI_M);

    /* Set period of 31 cycles (0 to 30 inclusive) */
    /* 125 MHz / 31 = 4.0322580645161 MHz */
    pwm_set_wrap(slice_num, 30);
    pwm_set_gpio_level(PIN_YMZ294_PHI_M, 15);
    pwm_set_enabled(slice_num, true);
}

static void psg_reset(void) {
}

static void psg_write(uint8_t addr, uint8_t data) {
    gpio_put(PIN_YMZ294_WRCS_N, true);
    gpio_put(PIN_YMZ294_AO, false);
    gpio_put(PIN_YMZ294_WRCS_N, false);
    gpio_put(PIN_YMZ294_D0, ((addr&(1u<<0))!=0) );
    gpio_put(PIN_YMZ294_D1, ((addr&(1u<<1))!=0) );
    gpio_put(PIN_YMZ294_D2, ((addr&(1u<<2))!=0) );
    gpio_put(PIN_YMZ294_D3, ((addr&(1u<<3))!=0) );
    gpio_put(PIN_YMZ294_D4, ((addr&(1u<<4))!=0) );
    gpio_put(PIN_YMZ294_D5, ((addr&(1u<<5))!=0) );
    gpio_put(PIN_YMZ294_D6, ((addr&(1u<<6))!=0) );
    gpio_put(PIN_YMZ294_D7, ((addr&(1u<<7))!=0) );
    gpio_put(PIN_YMZ294_WRCS_N, true);

    gpio_put(PIN_YMZ294_AO, true);
    gpio_put(PIN_YMZ294_WRCS_N, false);
    gpio_put(PIN_YMZ294_D0, ((data&(1u<<0))!=0) );
    gpio_put(PIN_YMZ294_D1, ((data&(1u<<1))!=0) );
    gpio_put(PIN_YMZ294_D2, ((data&(1u<<2))!=0) );
    gpio_put(PIN_YMZ294_D3, ((data&(1u<<3))!=0) );
    gpio_put(PIN_YMZ294_D4, ((data&(1u<<4))!=0) );
    gpio_put(PIN_YMZ294_D5, ((data&(1u<<5))!=0) );
    gpio_put(PIN_YMZ294_D6, ((data&(1u<<6))!=0) );
    gpio_put(PIN_YMZ294_D7, ((data&(1u<<7))!=0) );
    gpio_put(PIN_YMZ294_WRCS_N, true);
}

static bool repeating_timer_callback(struct repeating_timer *t) {
    (void)t;
    psgino_z.Proc();
    return true;
}

static int init(void) {
    psg_init();
    psgino_z.Initialize(psg_write, 125000000.0F/31/2, 100);
    psgino_z.Reset();
    add_repeating_timer_ms(-10, repeating_timer_callback, NULL, &timer);
    return 0;
}

static int deinit(void) {
    cancel_repeating_timer(&timer);
    uint slice_num = pwm_gpio_to_slice_num(PIN_YMZ294_PHI_M);
    pwm_set_enabled(slice_num, false);
    return 0;
}

static int play_bgm(mgc_sound_id_t bgm_id) {
    for ( size_t i = 0; i < bgm_list.record_count; i++ ) {
        const mgc_mml_record_t *record = &bgm_list.records[i];
        if ( record->id == bgm_id ) {
            psgino_z.SetMML(record->mml);
            psgino_z.Play();
            return 0;
        }
    }
    return -1;
}

static void stop_bgm(void) {
    psgino_z.Stop();
}

static int play_se(mgc_sound_id_t se_id) {
    for ( size_t i = 0; i < se_list.record_count; i++ ) {
        const mgc_mml_record_t *record = &se_list.records[i];
        if ( record->id == se_id ) {
            psgino_z.SetSeMML(record->mml);
            psgino_z.PlaySe();
            return 0;
        }
    }
    return -1;
}

static void stop_se(void) {
    psgino_z.StopSe();
}

static const mgc_sound_if_t common_if = {
    .init = init,
    .deinit = deinit,
    .play_bgm = play_bgm,
    .stop_bgm = stop_bgm,
    .play_se = play_se,
    .stop_se = stop_se,
};

static void set_bgm_list(const mgc_mml_record_t *records, size_t count) {
    bgm_list.records = records;
    bgm_list.record_count = count;
}

static void set_se_list(const  mgc_mml_record_t *records, size_t count) {
    se_list.records = records;
    se_list.record_count = count;
}

static void set_bgm_callback(void (*cb)(uint8_t ch, int32_t param)) {
    psgino_z.SetUserCallback(cb);
}

static void set_se_callback(void (*cb)(uint8_t ch, int32_t param)) {
    psgino_z.SetSeUserCallback(cb);
}

static void finish_primary_loop(void) {
    psgino_z.FinishPrimaryLoop();
}

static const mgc_sound_psg_if_t sound_psg_ymz294_rp2040 = {
    .common_if = &common_if,
    .set_bgm_list = set_bgm_list,
    .set_se_list = set_se_list,
    .set_bgm_callback = set_bgm_callback,
    .set_se_callback = set_se_callback,
    .finish_primary_loop = finish_primary_loop,
};

const mgc_sound_psg_if_t *sound_psg_ymz294_get_instance(void) {
    return &sound_psg_ymz294_rp2040;
}

