/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <hardware/gpio.h>
#include <hardware/adc.h>
#include "mgc/gamepad/sample/gamepad_sample.h"

#define NUM_OF_KEY_TABLE        (6)

#ifndef MGC_PIN_KEY0
#define MGC_PIN_KEY0            (0)
#endif/*MGC_PIN_KEY0*/

#ifndef MGC_PIN_KEY1
#define MGC_PIN_KEY1            (1)
#endif/*MGC_PIN_KEY1*/

#ifndef MGC_PIN_ADC0
#define MGC_PIN_ADC0            (26)
#endif/*MGC_PIN_ADC0*/

#ifndef MGC_PIN_ADC1
#define MGC_PIN_ADC1            (27)
#endif/*MGC_PIN_ADC1*/

#ifndef MGC_AXIS_X_ADC_CH
#define MGC_AXIS_X_ADC_CH       (0)
#endif/*MGC_AXIS_X_ADC_CH*/

#ifndef MGC_AXIS_Y_ADC_CH
#define MGC_AXIS_Y_ADC_CH       (1)
#endif/*MGC_AXIS_Y_ADC_CH*/

#ifndef MGC_AXIS_X_KEY_OFF_THRESHOLD
#define MGC_AXIS_X_KEY_OFF_THRESHOLD    (500L)
#endif/*MGC_AXIS_X_KEY_OFF_THRESHOLD*/

#ifndef MGC_AXIS_X_KEY_ON_THRESHOLD
#define MGC_AXIS_X_KEY_ON_THRESHOLD     (700L)
#endif/*MGC_AXIS_X_KEY_ON_THRESHOLD*/

#ifndef MGC_AXIS_Y_KEY_OFF_THRESHOLD
#define MGC_AXIS_Y_KEY_OFF_THRESHOLD    (500L)
#endif/*MGC_AXIS_Y_KEY_OFF_THRESHOLD*/

#ifndef MGC_AXIS_Y_KEY_ON_THRESHOLD
#define MGC_AXIS_Y_KEY_ON_THRESHOLD     (700L)
#endif/*MGC_AXIS_Y_KEY_ON_THRESHOLD*/

#ifndef MGC_KEY_COUNTER_THRESHOLD_0
#define MGC_KEY_COUNTER_THRESHOLD_0    (9)
#endif/*MGC_KEY_COUNTER_THRESHOLD_0*/

#ifndef MGC_KEY_COUNTER_THRESHOLD_1
#define MGC_KEY_COUNTER_THRESHOLD_1    (10)
#endif/*MGC_KEY_COUNTER_THRESHOLD_1*/

#ifndef MGC_KEY_COUNTER_THRESHOLD_2
#define MGC_KEY_COUNTER_THRESHOLD_2    (13)
#endif/*MGC_KEY_COUNTER_THRESHOLD_2*/

enum axis {
    AXIS_X = 0,
    AXIS_Y,
    AXIS_DIM
};

struct joystick_info {
    int16_t adc_value[AXIS_DIM];
    int16_t adc_offset[AXIS_DIM];
    int16_t axis_value[AXIS_DIM];
};

static struct joystick_info  joystick_info;

struct key_table {
    const enum gp_key      key;
    enum gp_key_state      key_stat;
    enum gp_key_state      prev_key_stat;
    enum gp_firing_state   firing_stat;
    bool                   is_key_state_changed;
    uint8_t                key_on_counter;
};

static struct key_table key_table[NUM_OF_KEY_TABLE] = {
    {GP_KEY_UP,     GP_KEY_STATE_OFF, GP_KEY_STATE_OFF, GP_FIRING_OFF, false, 0},
    {GP_KEY_DOWN,   GP_KEY_STATE_OFF, GP_KEY_STATE_OFF, GP_FIRING_OFF, false, 0},
    {GP_KEY_LEFT,   GP_KEY_STATE_OFF, GP_KEY_STATE_OFF, GP_FIRING_OFF, false, 0},
    {GP_KEY_RIGHT,  GP_KEY_STATE_OFF, GP_KEY_STATE_OFF, GP_FIRING_OFF, false, 0},
    {GP_KEY_0,      GP_KEY_STATE_OFF, GP_KEY_STATE_OFF, GP_FIRING_OFF, false, 0},
    {GP_KEY_1,      GP_KEY_STATE_OFF, GP_KEY_STATE_OFF, GP_FIRING_OFF, false, 0},
};

static struct key_table *get_key_table(enum gp_key key) {
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        if ( key_table[i].key == key ) {
            return &key_table[i];
        }
    }
    return NULL;
}

static int16_t get_adc_value(int adc_ch) {
    adc_select_input(adc_ch);
    return adc_read();
}

static void update_joystick_info(struct joystick_info *info) {
    // Axis-X
    info->adc_value[AXIS_X] = get_adc_value(MGC_AXIS_X_ADC_CH);
    info->axis_value[AXIS_X] = (info->adc_value[AXIS_X] - info->adc_offset[AXIS_X])*-1;

    // Axis-Y
    info->adc_value[AXIS_Y] = get_adc_value(MGC_AXIS_Y_ADC_CH);
    info->axis_value[AXIS_Y] = info->adc_value[AXIS_Y] - info->adc_offset[AXIS_Y];
}

static int init(void) {
    adc_init();
    adc_gpio_init(MGC_PIN_ADC0);
    adc_gpio_init(MGC_PIN_ADC1);

    gpio_init(MGC_PIN_KEY0);
    gpio_set_dir(MGC_PIN_KEY0, GPIO_IN);
    gpio_pull_up(MGC_PIN_KEY0);

    gpio_init(MGC_PIN_KEY1);
    gpio_set_dir(MGC_PIN_KEY1, GPIO_IN);
    gpio_pull_up(MGC_PIN_KEY1);

    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        key_table[i].key_stat = GP_KEY_STATE_OFF;
        key_table[i].firing_stat = GP_FIRING_OFF;
        key_table[i].is_key_state_changed = false;
        key_table[i].key_on_counter = 0;
    }

    // ADC calibration.
    // NOTE:
    // Please refrain from touching the joystick during the execute of this function.
    {
        int32_t sum;
        size_t n;
        for ( n = 0, sum = 0; n < 10; n++ ) {
            sum += get_adc_value(MGC_AXIS_X_ADC_CH);
        }
        joystick_info.adc_offset[AXIS_X] = (sum+5)/10;
        joystick_info.adc_value[AXIS_X] = joystick_info.adc_offset[AXIS_X];
        joystick_info.axis_value[AXIS_X] = 0;

        for ( n = 0, sum = 0; n < 10; n++ ) {
            sum += get_adc_value(MGC_AXIS_Y_ADC_CH);
        }
        joystick_info.adc_offset[AXIS_Y] = (sum+5)/10;
        joystick_info.adc_value[AXIS_Y] = joystick_info.adc_offset[AXIS_Y];
        joystick_info.axis_value[AXIS_Y] = 0;
    }
    return 0;
}

static int deinit(void) {
    return 0;
}

static int16_t get_x_axis(void) {
    return joystick_info.axis_value[AXIS_X];
}

static int16_t get_y_axis(void) {
    return joystick_info.axis_value[AXIS_Y];
}

static enum gp_key_state get_key_state(enum gp_key key) {
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        if ( key_table[i].key == key ) {
            return key_table[i].key_stat;
        }
    }
    return GP_KEY_STATE_OFF;
}

static bool is_key_state_changed(enum gp_key key) {
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        if ( key_table[i].key == key ) {
            return key_table[i].is_key_state_changed;
        }
    }
    return false;
}

static bool is_key_on_edge(enum gp_key key) {
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        if ( key_table[i].key == key ) {
            return (
                (key_table[i].key_stat == GP_KEY_STATE_ON) &&
                (key_table[i].is_key_state_changed == true)
            );
        }
    }
    return false;
}

static bool is_key_off_edge(enum gp_key key) {
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        if ( key_table[i].key == key ) {
            return (
                (key_table[i].key_stat == GP_KEY_STATE_OFF) &&
                (key_table[i].is_key_state_changed == true)
            );
        }
    }
    return false;
}

static enum gp_firing_state get_firing_state(enum gp_key key) {
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        if ( key_table[i].key == key ) {
            return key_table[i].firing_stat;
        }
    }
    return GP_FIRING_OFF;
}

static void proc(void) {
    struct key_table *tbl;
    // Update previous key state.
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        tbl = &key_table[i];
        tbl->prev_key_stat = tbl->key_stat;
    }

    // Update joystick info.
    update_joystick_info(&joystick_info);

    // Update key_left, and key_right.
    {
        int16_t x_axis = joystick_info.axis_value[AXIS_X];
        tbl = get_key_table(GP_KEY_LEFT);
        if ( tbl->key_stat == GP_KEY_STATE_OFF ) {
            if ( x_axis <= MGC_AXIS_X_KEY_ON_THRESHOLD*-1 ) tbl->key_stat = GP_KEY_STATE_ON;
        } else {
            if ( MGC_AXIS_X_KEY_OFF_THRESHOLD*-1 <= x_axis ) tbl->key_stat = GP_KEY_STATE_OFF;
        }
        tbl = get_key_table(GP_KEY_RIGHT);
        if ( tbl->key_stat == GP_KEY_STATE_OFF ) {
            if ( MGC_AXIS_X_KEY_ON_THRESHOLD <= x_axis ) tbl->key_stat = GP_KEY_STATE_ON;
        } else {
            if ( x_axis <= MGC_AXIS_X_KEY_OFF_THRESHOLD ) tbl->key_stat = GP_KEY_STATE_OFF;
        }
    }
    // Update key_down, and key_up.
    {
        int16_t y_axis = joystick_info.axis_value[AXIS_Y];
        tbl = get_key_table(GP_KEY_DOWN);
        if ( tbl->key_stat == GP_KEY_STATE_OFF ) {
            if ( y_axis <= MGC_AXIS_Y_KEY_ON_THRESHOLD*-1 ) tbl->key_stat = GP_KEY_STATE_ON;
        } else {
            if ( MGC_AXIS_Y_KEY_OFF_THRESHOLD*-1 <= y_axis ) tbl->key_stat = GP_KEY_STATE_OFF;
        }
        tbl = get_key_table(GP_KEY_UP);
        if ( tbl->key_stat == GP_KEY_STATE_OFF ) {
            if ( MGC_AXIS_Y_KEY_ON_THRESHOLD <= y_axis ) tbl->key_stat = GP_KEY_STATE_ON;
        } else {
            if ( y_axis <= MGC_AXIS_Y_KEY_OFF_THRESHOLD ) tbl->key_stat = GP_KEY_STATE_OFF;
        }
    }
    // Update key_0 and key_1.
    {
        tbl = get_key_table(GP_KEY_0);
        tbl->key_stat = (gpio_get(MGC_PIN_KEY0) == 0) ? GP_KEY_STATE_ON : GP_KEY_STATE_OFF;
        tbl = get_key_table(GP_KEY_1);
        tbl->key_stat = (gpio_get(MGC_PIN_KEY1) == 0) ? GP_KEY_STATE_ON : GP_KEY_STATE_OFF;
    }
    // Detect of key_stat changes.
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        tbl = &key_table[i];
        tbl->is_key_state_changed = (tbl->key_stat != tbl->prev_key_stat);
    }
    // Update firing states.
    for ( size_t i = 0; i < NUM_OF_KEY_TABLE; i++ ) {
        tbl = &key_table[i];
        if ( tbl->key_stat == GP_KEY_STATE_ON ) {
            if ( tbl->is_key_state_changed == true ) {
                tbl->firing_stat = GP_FIRING_ON;
                tbl->key_on_counter = 0;
            } else {
                tbl->key_on_counter++;
                if ( tbl->key_on_counter <= MGC_KEY_COUNTER_THRESHOLD_0 ) {
                    tbl->firing_stat = GP_FIRING_OFF;
                } else if (tbl->key_on_counter == MGC_KEY_COUNTER_THRESHOLD_1 ) {
                    tbl->firing_stat = GP_FIRING_ON;
                } else if (tbl->key_on_counter <= MGC_KEY_COUNTER_THRESHOLD_2 ) {
                    tbl->firing_stat = GP_FIRING_OFF;
                } else {
                    tbl->firing_stat = GP_FIRING_OFF;
                    tbl->key_on_counter = 9;
                }
            }
        } else {
            tbl->firing_stat = GP_FIRING_OFF;
            tbl->key_on_counter = 0;
        }
    }
}

static const mgc_gamepad_if_t gamepad_sample_rp2040 = {
    .init = init,
    .deinit = deinit,
    .get_x_axis = get_x_axis,
    .get_y_axis = get_y_axis,
    .get_key_state = get_key_state,
    .is_key_state_changed = is_key_state_changed,
    .is_key_on_edge = is_key_on_edge,
    .is_key_off_edge = is_key_off_edge,
    .get_firing_state = get_firing_state,
    .proc = proc,
};

const mgc_gamepad_if_t *gamepad_sample_get_instance(void) {
    return &gamepad_sample_rp2040;
}

