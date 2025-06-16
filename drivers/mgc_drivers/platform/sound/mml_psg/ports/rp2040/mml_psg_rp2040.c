/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include <pico/stdlib.h>
#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/irq.h>
#include "mgc_drivers/platform/sound/mml_psg/mml_psg_local.h"

#ifndef MGC_DRIVERS_MML_PSG_RP2040_ALARM_NUM
#define MGC_DRIVERS_MML_PSG_RP2040_ALARM_NUM                1
#endif/*MGC_DRIVERS_MML_PSG_RP2040_ALARM_NUM*/

#ifndef MGC_DRIVERS_MML_PSG_RP2040_ALARM_IRQ_PRI
#define MGC_DRIVERS_MML_PSG_RP2040_ALARM_IRQ_PRI            0xC0
#endif/*MGC_DRIVERS_MML_PSG_RP2040_ALARM_IRQ_PRI*/

// NOTE: PWM interrupt priority must be higher than ALARM to ensure accurate PSG timing.
#ifndef MGC_DRIVERS_MML_PSG_RP2040_PWM_IRQ_PRI
#define MGC_DRIVERS_MML_PSG_RP2040_PWM_IRQ_PRI              0x80
#endif/*MGC_DRIVERS_MML_PSG_RP2040_PWM_IRQ_PRI*/

#ifndef MGC_DRIVERS_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT
#define MGC_DRIVERS_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT       28
#endif/*MGC_DRIVERS_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT*/


#define ALARM_NUM           MGC_DRIVERS_MML_PSG_RP2040_ALARM_NUM
#define ALARM_IRQ           timer_hardware_alarm_get_irq_num(timer_hw, ALARM_NUM)
#define ALARM_IRQ_PRI       MGC_DRIVERS_MML_PSG_RP2040_ALARM_IRQ_PRI

#define PWM_PIN_OUTPUT      MGC_DRIVERS_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT
#define PWM_IRQ_PRI         MGC_DRIVERS_MML_PSG_RP2040_PWM_IRQ_PRI
#define PWM_MAX_WRAP        0xFFFF           // 16-bit max wrap

static bool irq_enabled_prev;
static uint64_t mml_proc_interval;


static void pwm_irq_wrap_handler(void) {

    uint16_t level = mml_psg_local__proc_psg_emu();
    pwm_set_gpio_level(PWM_PIN_OUTPUT, level);

    pwm_clear_irq(pwm_gpio_to_slice_num(PWM_PIN_OUTPUT));
}

static void alarm_irq_handler(void) {
    hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);
    hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);
    uint64_t target = timer_hw->timerawl + mml_proc_interval;
    timer_hw->alarm[ALARM_NUM] = (uint32_t) target;

    mml_psg_local__proc_psg_mml();
}

void mml_psg_port__init(float psg_emu_rate, float mml_proc_rate) {

    /* Configure PSG Emulation Process Rate */
    {
        if ( psg_emu_rate <= 1000.0F ) {
            psg_emu_rate = 1000.0F;
        } else if ( psg_emu_rate >= 50000.0F ) {
            psg_emu_rate = 50000.0F;
        } else {
        }

        uint slice_num = pwm_gpio_to_slice_num(PWM_PIN_OUTPUT);
        gpio_set_function(PWM_PIN_OUTPUT, GPIO_FUNC_PWM);

        uint32_t pwm_clk_hz = clock_get_hz(clk_sys);
        float clkdiv = 1.0f;
        uint32_t wrap = pwm_clk_hz / (psg_emu_rate * clkdiv);

        while (wrap > PWM_MAX_WRAP && clkdiv < 256.0f) {
            clkdiv += 0.5f;
            wrap = (uint32_t)((float)pwm_clk_hz / (psg_emu_rate * clkdiv));
        }

        if (wrap > PWM_MAX_WRAP) {
            wrap = PWM_MAX_WRAP;
        }

        pwm_set_clkdiv(slice_num, clkdiv);
        pwm_set_wrap(slice_num, wrap);
        pwm_set_gpio_level(PWM_PIN_OUTPUT, 0);

        pwm_clear_irq(slice_num);
        pwm_set_enabled(slice_num, true);
        pwm_set_irq_enabled(slice_num, true);

        irq_set_priority(PWM_IRQ_WRAP, PWM_IRQ_PRI);
        irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_irq_wrap_handler);
        irq_set_enabled(PWM_IRQ_WRAP, true);

        irq_enabled_prev = irq_is_enabled(PWM_IRQ_WRAP);
    }

    /* Configure MML Process Rate */
    {
        hardware_alarm_claim(ALARM_NUM);

        hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);

        irq_set_exclusive_handler(ALARM_IRQ, alarm_irq_handler);
        irq_set_priority(ALARM_IRQ, ALARM_IRQ_PRI);
        irq_set_enabled(ALARM_IRQ, true);

        if ( mml_proc_rate <= 10.0F ) {
            mml_proc_interval = 100 * 1000;
        } else if ( mml_proc_rate >= 1000000.0F ) {
            mml_proc_interval = 1;
        } else {
            mml_proc_interval = (uint64_t)((1000*1000.0F/mml_proc_rate) + 0.5F);
        }

        uint64_t target = timer_hw->timerawl + mml_proc_interval;
        timer_hw->alarm[ALARM_NUM] = (uint32_t) target;
    }
}

void mml_psg_port__deinit(void) {

    hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);
    irq_set_enabled(ALARM_IRQ, false);

    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN_OUTPUT);
    irq_set_enabled(PWM_IRQ_WRAP, false);
    pwm_set_irq_enabled(slice_num, false);
    pwm_set_enabled(slice_num, false);
}

void mml_psg_port__psg_guard_enter(void) {
    irq_enabled_prev = irq_is_enabled(PWM_IRQ_WRAP);
    irq_set_enabled(PWM_IRQ_WRAP, false);
}

void mml_psg_port__psg_guard_exit(void) {
    irq_set_enabled(PWM_IRQ_WRAP, irq_enabled_prev);
}

