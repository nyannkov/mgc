/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_ANIM_FRAMES_H
#define MGC_ANIM_FRAMES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/components/tileset.h"

typedef struct mgc_anim_frame {
    size_t tile_index;
    uint32_t time_ms;
} mgc_anim_frame_t;

typedef struct mgc_anim_frames {
    const mgc_tileset_t* tileset;
    const mgc_anim_frame_t* frame_array;
    size_t frame_count;
} mgc_anim_frames_t;

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_ANIM_FRAMES_H*/
