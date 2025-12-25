#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "mgc_drivers/platform/sound/mml_psg/mml_psg_local.h"

#define CHUNK_SAMPLES   512 
#define LOW_WATERMARK   (Uint32)(CHUNK_SAMPLES * sizeof(int16_t) * 2)

static SDL_AudioSpec want = {0};
static SDL_AudioSpec have = {0};
static SDL_AudioDeviceID dev;

int sample_accum;
int mml_interval;

int32_t last_x;
int32_t last_y;

void mml_psg_port__init(float psg_emu_rate, float mml_proc_rate) {

    mml_interval = (int)( psg_emu_rate / mml_proc_rate );

    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec want = {0};
    SDL_AudioSpec have = {0};

    want.freq = (int)psg_emu_rate;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 1024;
    want.callback = NULL;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    SDL_PauseAudioDevice(dev, 0);
}

void mml_psg_port__deinit(void) {

    SDL_CloseAudioDevice(dev);
    SDL_Quit();
}

void mml_psg_port__psg_guard_enter(void) {
}

void mml_psg_port__psg_guard_exit(void) {
}

void mml_psg_sim_proc(void) {

    if (dev == 0) {
        return;
    }

    Uint32 queued_bytes = SDL_GetQueuedAudioSize(dev);
    Uint32 queued_samples = queued_bytes / sizeof(int16_t);

    while (queued_samples < LOW_WATERMARK) {

        int16_t buffer[CHUNK_SAMPLES];

        for (size_t i = 0; i < CHUNK_SAMPLES; i++) {

            sample_accum++;
            if (sample_accum >= mml_interval) {
                sample_accum -= mml_interval;
                mml_psg_local__proc_psg_mml();
            }

            int16_t level = mml_psg_local__proc_psg_emu();

            last_y = last_x - level + 0.995 * last_y;
            last_x = level;

            buffer[i] = last_y * 4;
        }

        SDL_QueueAudio(dev, buffer, sizeof(buffer));
        queued_samples += CHUNK_SAMPLES;
    }
}

void mml_psg_sim_deinit(void) {
    SDL_CloseAudioDevice(dev);
}

