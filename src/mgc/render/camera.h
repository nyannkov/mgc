/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_CAMERA_H
#define MGC_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"
#include "mgc/components/sprite.h"
#include "pixelbuffer.h"

typedef struct mgc_follow_settings {
    int16_t follow_line;
    int16_t start_line;
    int16_t end_line;
    uint16_t deadzone;
} mgc_follow_settings_t;

typedef struct mgc_camera {
    mgc_follow_settings_t x_settings;
    mgc_follow_settings_t y_settings;
    bool x_enabled;
    bool y_enabled;
    int16_t x_follow_line;
    int16_t y_follow_line;
    int16_t x;
    int16_t y;
} mgc_camera_t;

void camera_init(mgc_camera_t *camera);
void camera_set_x_follow_settings(mgc_camera_t *camera, int16_t start_line, int16_t end_line, uint16_t deadzone);
void camera_set_y_follow_settings(mgc_camera_t *camera, int16_t start_line, int16_t end_line, uint16_t deadzone);
void camera_set_x_follow_enabled(mgc_camera_t *camera, bool enabled);
void camera_set_y_follow_enabled(mgc_camera_t *camera, bool enabled);
void camera_follow_target(mgc_camera_t *camera, const mgc_sprite_t *target);
bool camera_get_position(const mgc_camera_t *camera, mgc_point_t *cam_pos);

// legacy
void camera_update(mgc_pixelbuffer_t *pixelbuffer, mgc_camera_t *camera, const mgc_sprite_t *target);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_CAMERA_H*/
