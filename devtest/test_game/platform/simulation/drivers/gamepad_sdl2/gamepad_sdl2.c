#include <SDL2/SDL.h>
#include <stdbool.h>
#include "gamepad_sdl2.h"

#define TOTAL_KEY_PAIRS    9  

typedef struct key_pair {
    enum mgc_gamepad_key key;
    SDL_Scancode code;
} key_pair_t;

static const key_pair_t key_pairs[TOTAL_KEY_PAIRS]  = {
    {   MGC_GAMEPAD_KEY_UP      ,SDL_SCANCODE_UP     },
    {   MGC_GAMEPAD_KEY_DOWN    ,SDL_SCANCODE_DOWN   },
    {   MGC_GAMEPAD_KEY_RIGHT   ,SDL_SCANCODE_RIGHT  },
    {   MGC_GAMEPAD_KEY_LEFT    ,SDL_SCANCODE_LEFT   },
    {   MGC_GAMEPAD_KEY_ENTER   ,SDL_SCANCODE_Z      }, // keyA
    {   MGC_GAMEPAD_KEY_CANCEL  ,SDL_SCANCODE_X      }, // keyB
    {   MGC_GAMEPAD_KEY_MENU    ,SDL_SCANCODE_A      }, // keyX
    {   MGC_GAMEPAD_KEY_HOME    ,SDL_SCANCODE_S      }, // keyY
    {   MGC_GAMEPAD_KEY_CONTROL ,SDL_SCANCODE_LCTRL  }
};

static mgc_gamepad_key_state_t key_states[TOTAL_KEY_PAIRS];


static bool sdl2_key_get(void *ctx, enum mgc_gamepad_key key) {

    (void)ctx;

    SDL_Scancode sc = SDL_SCANCODE_UNKNOWN;

    for ( size_t i = 0; i < TOTAL_KEY_PAIRS; i++ ) {
        if ( key_pairs[i].key == key ) {
            sc = key_pairs[i].code;
            break;
        }
    }

    if ( sc != SDL_SCANCODE_UNKNOWN ) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        return (keys[sc] != 0);
    }

    return false;
}

static void sdl2_key_poll(void *ctx) {
    (void)ctx;
    SDL_PumpEvents();
}

static const mgc_gamepad_source_t source = {
    .get = sdl2_key_get,
    .poll = sdl2_key_poll,
    .ctx = NULL 
};

void gamepad_sdl2_init(mgc_gamepad_t *gamepad) {
    
    for ( size_t i = 0; i < TOTAL_KEY_PAIRS; i++ ) {
        key_states[i].key = key_pairs[i].key;
    }

    gamepad_init(gamepad, &source, key_states, countof(key_states));
}

