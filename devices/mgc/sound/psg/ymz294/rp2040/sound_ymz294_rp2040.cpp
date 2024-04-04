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
#include "sound_ymz294_rp2040.h"

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

struct sound_table {
    const struct mml_sound_record *records;
    size_t record_count;
};
static struct sound_table bgm_table, se_table;

static struct repeating_timer timer;

static void psg_write(uint8_t addr, uint8_t data);
PsginoZ psgino_z = PsginoZ(psg_write, 125000000.0F/31/2, 100);

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
    bgm_table.records = NULL;
    bgm_table.record_count = 0;
    se_table.records = NULL;
    se_table.record_count = 0;
    psg_init();
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
    for ( size_t i = 0; i < bgm_table.record_count; i++ ) {
        const struct mml_sound_record *record = &bgm_table.records[i];
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
    for ( size_t i = 0; i < se_table.record_count; i++ ) {
        const struct mml_sound_record *record = &se_table.records[i];
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

const struct mgc_sound_if sound_driver_ymz294_rp2040 = {
    .init = init,
    .deinit = deinit,
    .play_bgm = play_bgm,
    .stop_bgm = stop_bgm,
    .play_se = play_se,
    .stop_se = stop_se,
};

void sound_driver_ymz294_rp2040_set_bgm_table(const mml_sound_record_t *records, size_t count) {
    bgm_table.records = records;
    bgm_table.record_count = count;
}

void sound_driver_ymz294_rp2040_set_se_table(const  mml_sound_record_t *records, size_t count) {
    se_table.records = records;
    se_table.record_count = count;
}

