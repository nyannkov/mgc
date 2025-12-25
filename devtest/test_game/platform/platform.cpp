#include "platform.hpp"

#if defined(MGC_SIMULATION)
#include <SDL2/SDL.h>
#include <unistd.h>
#include <chrono>
#include <stdbool.h>

extern "C" void mml_psg_sim_proc(void);
#endif


namespace app {

void platform_init() {
    display_driver.init(50*1000*1000); // over-clock
    sound_controller.init();
    gamepad.init();
    frame_timer.reset();
}

void platform_sleep(unsigned int ms) {
#if defined(MGC_SIMULATION)
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = ms * 1000 * 1000;
    nanosleep(&ts, NULL);
#else
    (void)ms;
#endif
}

void platform_gamepad_proc() {
#if defined(MGC_SIMULATION)
    SDL_PumpEvents();
#endif
    app::gamepad.proc();
}

void platform_sound_proc() {
#if defined(MGC_SIMULATION)
    mml_psg_sim_proc();
#endif
}

void platform_tick() {
    frame_timer.tick();
}

bool platform_is_continue() {
    bool is_continue = true;
#if defined(MGC_SIMULATION)
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if ( e.type == SDL_QUIT ) {
            is_continue = false;
            break;
        }
    }
#endif
    return is_continue;
}

void platform_deinit() {
#if defined(MGC_SIMULATION)
    SDL_Quit();
#endif
}


} // namespace app

