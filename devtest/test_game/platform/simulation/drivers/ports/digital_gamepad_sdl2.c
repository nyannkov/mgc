#include <SDL2/SDL.h>
#include <stdbool.h>
#include "mgc_drivers/platform/input/digital_gamepad/ports/digital_gamepad_port.h"

static const struct key_pair {
    SDL_Scancode code;
    unsigned int pin;
} key_table[] = {
    { SDL_SCANCODE_UP,       2 },
    { SDL_SCANCODE_DOWN,    18 },
    { SDL_SCANCODE_RIGHT,   20 },
    { SDL_SCANCODE_LEFT,    16 },
    { SDL_SCANCODE_Z,       15 }, // keyA
    { SDL_SCANCODE_X,       17 }, // keyB
    { SDL_SCANCODE_A,       19 }, // keyX
    { SDL_SCANCODE_S,       21 }, // keyY
    { SDL_SCANCODE_LCTRL,    3 }
};

void digital_gamepad_port__gpio_init(unsigned int pin) {
    (void)pin;
}

void digital_gamepad_port__gpio_set_input_mode(unsigned int pin, bool pull_up) {
    (void)pin;
    (void)pull_up;
}

bool digital_gamepad_port__gpio_get(unsigned int pin) {
    
    SDL_Scancode sc = SDL_SCANCODE_UNKNOWN;

    for ( size_t i = 0; i < sizeof(key_table)/sizeof(key_table[0]); i++ ) {
        if ( key_table[i].pin == pin ) {
            sc = key_table[i].code;
            break;
        }
    }

    if ( sc != SDL_SCANCODE_UNKNOWN ) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        return (keys[sc] == 0);
    }

    return true;
}

