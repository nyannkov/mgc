#include <stdio.h>
#include <chrono>
#include <SDL2/SDL.h>
#include "mgc_cpp/mgc.hpp"
#include "sound_controller_mml_psg.hpp"

extern "C" void mml_psg_sim_proc(void);

namespace {
using SoundControllerT = mgc::drivers::platform::sound::MmlPsgSoundController;
SoundControllerT sound;

void platform_sleep_ms(unsigned int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, NULL);
}

}

int main(int argc, char* argv[]) {
    
    if ( argc < 2 ) {
        return 1;
    }

    const char * mml = argv[1];

    sound.init();

    sound.play_background_music(mml);

    while (1) {
        
        mml_psg_sim_proc();

        if ( sound.has_background_music_finished() ) {
            break;
        }

        platform_sleep_ms(1);
    }

    platform_sleep_ms(1000);
    SDL_Quit();

    return 0;
}

