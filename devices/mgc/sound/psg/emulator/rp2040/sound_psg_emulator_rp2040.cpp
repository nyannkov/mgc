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
#include "mgc/sound/psg/emulator/emu2149/emu2149.h"
#include "mgc/sound/psg/emulator/sound_psg_emulator.h"

#ifndef MGC_PIN_PSG_EMU_OUTPUT
#define MGC_PIN_PSG_EMU_OUTPUT          (28)
#endif/*MGC_PIN_PSG_EMU_OUTPUT*/

#ifndef MGC_EMU2149_CLOCK
#define MGC_EMU2149_CLOCK               (2000000)
#endif/*MGC_EMU2149_CLOCK*/

static PSG *psg;
static PsginoZ psgino_z;
static struct repeating_timer timer;
static mgc_mml_list_t bgm_list, se_list;

static void pwm_irq_wrap_handler(void) {
    if ( psg ) {
        int16_t output;
        output = PSG_calc(psg);
        pwm_set_gpio_level(MGC_PIN_PSG_EMU_OUTPUT, output>>6);
    }
    pwm_clear_irq(pwm_gpio_to_slice_num(MGC_PIN_PSG_EMU_OUTPUT));
}

static bool repeating_timer_callback(struct repeating_timer *t) {
    (void)t;
    psgino_z.Proc();
    return true;
}

static void init_hw(void) {

    /* Repeating-Timer */
    {
        uint alarm_num;
        add_repeating_timer_ms(-10, repeating_timer_callback, NULL, &timer);
        // Set the priority of TIMER_IRQ_x lower than that of PWM_IRQ_WRAP.
        alarm_num = alarm_pool_hardware_alarm_num(timer.pool);
        irq_set_priority(TIMER_IRQ_0 + alarm_num, 0xC0);
    }

    /* PWM */
    {
        uint slice_num;

        slice_num = pwm_gpio_to_slice_num(MGC_PIN_PSG_EMU_OUTPUT);

        gpio_set_function(MGC_PIN_PSG_EMU_OUTPUT, GPIO_FUNC_PWM);

        /* 125 MHz / (3124+1) = 40 kHz */
        pwm_set_wrap(slice_num, 3124);

        pwm_set_gpio_level(MGC_PIN_PSG_EMU_OUTPUT, 0);

        pwm_clear_irq(slice_num);
        pwm_set_enabled(slice_num, true);
        pwm_set_irq_enabled(slice_num, true);

        irq_set_priority(PWM_IRQ_WRAP, 0x80);
        irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_irq_wrap_handler);
        irq_set_enabled(PWM_IRQ_WRAP, true);
    }
}

static void psg_write(uint8_t addr, uint8_t data) {
    bool irq_enabled = irq_is_enabled(PWM_IRQ_WRAP);
    irq_set_enabled(PWM_IRQ_WRAP, false);
    PSG_writeReg(psg, addr, data);
    irq_set_enabled(PWM_IRQ_WRAP, irq_enabled);
}

static int init(void) {
    if ( !psg ) {
        psg = PSG_new(MGC_EMU2149_CLOCK, 40000);
        psgino_z.Initialize(psg_write, MGC_EMU2149_CLOCK);
        init_hw();
        return 0;
    }
    return 1;
}

static int deinit(void) {

    uint slice_num = pwm_gpio_to_slice_num(MGC_PIN_PSG_EMU_OUTPUT);

    cancel_repeating_timer(&timer);
    irq_set_enabled(PWM_IRQ_WRAP, false);
    pwm_set_irq_enabled(slice_num, false);
    pwm_set_enabled(slice_num, false);

    if ( psg ) {
        PSG_delete(psg);
        psg = NULL;
        return 0;
    }
    return 1;
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
        const mgc_mml_record *record = &se_list.records[i];
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

static const mgc_sound_if_t sound_psg_emulator_rp2040 = {
    .init = init,
    .deinit = deinit,
    .play_bgm = play_bgm,
    .stop_bgm = stop_bgm,
    .play_se = play_se,
    .stop_se = stop_se,
};

const mgc_sound_if_t *sound_psg_emulator_get_instance(void) {
    return &sound_psg_emulator_rp2040;
}

void sound_psg_emulator_set_bgm_list(const mgc_mml_record_t *records, size_t count) {
    bgm_list.records = records;
    bgm_list.record_count = count;
}

void sound_psg_emulator_set_se_list(const  mgc_mml_record_t *records, size_t count) {
    se_list.records = records;
    se_list.record_count = count;
}

