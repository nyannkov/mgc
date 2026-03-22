#include "gamepad_picolcd.h"
#include "hardware/gpio.h"

#define TOTAL_PIN_CONFIG    9  

typedef struct pin_config {
    enum mgc_gamepad_key key;
    uint pin;
} pin_config_t;

/*****************************************************************************
* Configuration for Waveshare Pico-LCD-1.3
*
* Reference:
* https://www.waveshare.com/wiki/Pico-LCD-1.3
******************************************************************************/
static const pin_config_t pin_conf[TOTAL_PIN_CONFIG]  = {
    { MGC_GAMEPAD_KEY_UP,       2 },
    { MGC_GAMEPAD_KEY_DOWN,    18 },
    { MGC_GAMEPAD_KEY_RIGHT,   20 },
    { MGC_GAMEPAD_KEY_LEFT,    16 },
    { MGC_GAMEPAD_KEY_ENTER,   15 }, // keyA
    { MGC_GAMEPAD_KEY_CANCEL,  17 }, // keyB
    { MGC_GAMEPAD_KEY_MENU,    19 }, // keyX
    { MGC_GAMEPAD_KEY_HOME,    21 }, // keyY
    { MGC_GAMEPAD_KEY_CONTROL,  3 }
};

static mgc_gamepad_key_state_t key_states[TOTAL_PIN_CONFIG];


static bool picolcd_key_get(void *ctx, enum mgc_gamepad_key key) {
    (void)ctx;

    for ( size_t i = 0; i < TOTAL_PIN_CONFIG; i++ ) {
        if ( pin_conf[i].key == key ) {
            return ( gpio_get(pin_conf[i].pin) == 0 );
        }
    }

    return false;
}

static void picolcd_key_poll(void *ctx) {
    (void)ctx;
}

static const mgc_gamepad_source_t source = {
    .get = picolcd_key_get,
    .poll = picolcd_key_poll,
    .ctx = NULL 
};

void gamepad_picolcd_init(mgc_gamepad_t *gamepad) {
    
    for ( size_t i = 0; i < TOTAL_PIN_CONFIG; i++ ) {
        int pin = pin_conf[i].pin;
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);

        key_states[i].key = pin_conf[i].key;
    }

    gamepad_init(gamepad, &source, key_states, countof(key_states));
}

