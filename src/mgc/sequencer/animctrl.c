#include "animctrl.h"


void animctrl_init(mgc_animctrl_t* animctrl) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    
    animctrl->frames = NULL;
    animctrl->current_frame_index = 0;
    animctrl->loop = 0;
    animctrl->speed = 1.0f;
    animctrl->frame_anchor_ms = 0;
    animctrl->frame_elapsed_ms = 0;
    animctrl->state = MGC_ANIMCTRL_STATE_INIT;
}

void animctrl_set_anim_frames(mgc_animctrl_t* animctrl, const mgc_anim_frames_t* frames) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    animctrl->frames = frames;

    animctrl_reset(animctrl);
}

void animctrl_set_speed(mgc_animctrl_t* animctrl, float speed) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    animctrl->speed = speed;
}

void animctrl_set_loop(mgc_animctrl_t* animctrl, bool loop) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    animctrl->loop = loop;
}

void animctrl_reset(mgc_animctrl_t* animctrl) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    const mgc_anim_frames_t* frames = animctrl->frames;
    if ( frames != NULL ) {

        if ( animctrl->speed >= 0.0f ) {
            animctrl->current_frame_index = 0;
        } else {
            animctrl->current_frame_index = frames->frame_count - 1;
        }

        animctrl->frame_elapsed_ms = 0;
    }

    animctrl->state = MGC_ANIMCTRL_STATE_INIT;
}

void animctrl_start(mgc_animctrl_t* animctrl, uint32_t timer_now_ms) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    if ( animctrl->state == MGC_ANIMCTRL_STATE_INIT ) {
        animctrl->frame_anchor_ms = timer_now_ms;
        animctrl->frame_elapsed_ms = 0;
        animctrl->state = MGC_ANIMCTRL_STATE_IN_PROGRESS;
    }
}

void animctrl_pause(mgc_animctrl_t* animctrl) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( animctrl->state == MGC_ANIMCTRL_STATE_IN_PROGRESS ) {
        animctrl->state = MGC_ANIMCTRL_STATE_PAUSED;
    }
}

void animctrl_resume(mgc_animctrl_t* animctrl, uint32_t timer_now_ms) {
    if ( animctrl == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( animctrl->state == MGC_ANIMCTRL_STATE_PAUSED ) {
        animctrl->state = MGC_ANIMCTRL_STATE_IN_PROGRESS;
        animctrl->frame_anchor_ms = timer_now_ms;
    }
}

/**
 * @brief Advances at most one frame per call.
 *        Even if a long time has passed, frames are not skipped.
 *
 * @return true if the current frame has changed
 */
bool animctrl_proc(mgc_animctrl_t* animctrl, uint32_t timer_now_ms) {
    
    if ( ( animctrl == NULL ) ||
         ( animctrl->frames == NULL ) 
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    if ( animctrl->state != MGC_ANIMCTRL_STATE_IN_PROGRESS ) {
        return false;
    }

    const mgc_anim_frame_t* current_frame 
        = &animctrl->frames->frame_array[animctrl->current_frame_index];

    uint32_t delta_ms = timer_now_ms - animctrl->frame_anchor_ms;
    uint32_t elapsed_ms = animctrl->frame_elapsed_ms + delta_ms;

    animctrl->frame_anchor_ms = timer_now_ms;

    bool reverse = (animctrl->speed < 0.0f);
    float speed_abs = MGC_ABS(animctrl->speed);

    elapsed_ms = (uint32_t)(elapsed_ms * speed_abs);

    bool frame_changed = true;

    if ( elapsed_ms < current_frame->time_ms ) {

        frame_changed = false;

        animctrl->frame_elapsed_ms = elapsed_ms;
        
    } else {

        animctrl->frame_elapsed_ms = elapsed_ms - current_frame->time_ms;

        if ( reverse ) {
            if ( animctrl->current_frame_index > 0 ) {
                animctrl->current_frame_index--;
            } else {

                if ( animctrl->loop ) {
                    animctrl->current_frame_index = animctrl->frames->frame_count - 1;
                } else {
                    animctrl->state = MGC_ANIMCTRL_STATE_FINISHED;
                    frame_changed = false;
                }
            }
        } else {
            if ( animctrl->current_frame_index < (animctrl->frames->frame_count - 1) ) {
                animctrl->current_frame_index++;
            } else {

                if ( animctrl->loop ) {
                    animctrl->current_frame_index = 0;
                } else {
                    animctrl->state = MGC_ANIMCTRL_STATE_FINISHED;
                    frame_changed = false;
                }
            }
        }
    }

    return frame_changed;
}

void animctrl_seek_to_time_ms(mgc_animctrl_t* animctrl, uint32_t time_ms) {
    if ((animctrl == NULL) || (animctrl->frames == NULL)) {
        MGC_WARN("Invalid handler");
        return;
    }

    const mgc_anim_frames_t* frames = animctrl->frames;
    uint32_t t0 = 0;

    for (size_t i = 0; i < frames->frame_count; i++) {
        const mgc_anim_frame_t* fr = &frames->frame_array[i];
        uint32_t t1 = t0 + fr->time_ms;

        if (time_ms < t1) {
            animctrl->current_frame_index = i;
            animctrl->frame_elapsed_ms = time_ms - t0;
            return;
        }

        t0 = t1;
    }

    animctrl->current_frame_index = frames->frame_count - 1;
    animctrl->frame_elapsed_ms = frames->frame_array[frames->frame_count - 1].time_ms;
}

void animctrl_seek_to_frame(mgc_animctrl_t* animctrl, size_t frame_index) {
    if ( ( animctrl == NULL ) ||
         ( animctrl->frames == NULL ) 
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    const mgc_anim_frames_t* frames = animctrl->frames;

    if ( frames->frame_count <= frame_index ) {
        animctrl->current_frame_index = frames->frame_count - 1;
    } else {
        animctrl->current_frame_index = frame_index;
    }

    animctrl->frame_elapsed_ms = 0;
}

