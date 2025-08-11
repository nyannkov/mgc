/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ANIMCTRL_H
#define MGC_ANIMCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "anim_frames.h"

enum mgc_animctrl_state {
    MGC_ANIMCTRL_STATE_INIT = 0,
    MGC_ANIMCTRL_STATE_IN_PROGRESS,
    MGC_ANIMCTRL_STATE_PAUSED,
    MGC_ANIMCTRL_STATE_FINISHED
};

typedef struct mgc_animctrl {
    const mgc_anim_frames_t* frames;
    size_t current_frame_index;
    bool loop;
    float speed;
    uint32_t frame_anchor_ms;
    uint32_t frame_elapsed_ms;
    enum mgc_animctrl_state state;
} mgc_animctrl_t;

void animctrl_init(mgc_animctrl_t* animctrl);
void animctrl_set_anim_frames(mgc_animctrl_t* animctrl, const mgc_anim_frames_t* frames);
void animctrl_set_speed(mgc_animctrl_t* animctrl, float speed);
void animctrl_set_loop(mgc_animctrl_t* animctrl, bool loop);
void animctrl_reset(mgc_animctrl_t* animctrl);
void animctrl_start(mgc_animctrl_t* animctrl, uint32_t timer_now_ms);
void animctrl_pause(mgc_animctrl_t* animctrl);
void animctrl_resume(mgc_animctrl_t* animctrl, uint32_t timer_now_ms);
bool animctrl_proc(mgc_animctrl_t* animctrl, uint32_t timer_now_ms);
void animctrl_seek_to_time_ms(mgc_animctrl_t* animctrl, uint32_t time_ms);
void animctrl_seek_to_frame(mgc_animctrl_t* animctrl, size_t frame_index);

static inline
const mgc_tileset_t* animctrl_get_tileset(const mgc_animctrl_t* animctrl) {

    MGC_ASSERT(animctrl != NULL , "Invalid handler");

    const mgc_anim_frames_t* frames = animctrl->frames;

    if ( frames != NULL ) {
        return frames->tileset;
    } else {
        return NULL;
    }
}

static inline
size_t animctrl_get_current_tile_index(const mgc_animctrl_t* animctrl) {

    MGC_ASSERT(animctrl != NULL , "Invalid handler");

    const mgc_anim_frames_t* frames = animctrl->frames;

    if ( frames != NULL ) {
        return frames->frame_array[animctrl->current_frame_index].tile_index;
    } else {
        return 0;
    }
}

static inline
bool animctrl_is_finished(const mgc_animctrl_t* animctrl) {

    MGC_ASSERT(animctrl != NULL , "Invalid handler");

    return animctrl->state == MGC_ANIMCTRL_STATE_FINISHED;
}

static inline
bool animctrl_is_paused(const mgc_animctrl_t* animctrl) {

    MGC_ASSERT(animctrl != NULL , "Invalid handler");

    return animctrl->state == MGC_ANIMCTRL_STATE_PAUSED;
}

static inline
enum mgc_animctrl_state animctrl_get_state(const mgc_animctrl_t* animctrl) {

    MGC_ASSERT(animctrl != NULL , "Invalid handler");

    return animctrl->state;
}

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_ANIMCTRL_H*/
