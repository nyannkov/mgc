/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "camera.h"

void camera_init(mgc_camera_t *camera) {
    if ( camera == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    camera->x_enabled = false;
    camera->y_enabled = false;
    camera->x_follow_line = 0;
    camera->y_follow_line = 0;
    camera_set_x_follow_settings(camera, 0, 0, 0);
    camera_set_y_follow_settings(camera, 0, 0, 0);
    camera->x = 0;
    camera->y = 0;
}

void camera_set_x_follow_settings(mgc_camera_t *camera, int16_t start_line, int16_t end_line, uint16_t deadzone) {
    if ( camera == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    camera->x_settings.start_line = start_line;
    camera->x_settings.end_line = end_line;
    camera->x_settings.deadzone = deadzone;
    camera->x_follow_line = start_line;
}

void camera_set_y_follow_settings(mgc_camera_t *camera, int16_t start_line, int16_t end_line, uint16_t deadzone) {
    if ( camera == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    camera->y_settings.start_line = start_line;
    camera->y_settings.end_line = end_line;
    camera->y_settings.deadzone = deadzone;
    camera->y_follow_line = start_line;
}

void camera_set_x_follow_enabled(mgc_camera_t *camera, bool enabled) {
    if ( camera == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    camera->x_enabled = enabled;
}

void camera_set_y_follow_enabled(mgc_camera_t *camera, bool enabled) {
    if ( camera == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    camera->y_enabled = enabled;
}

void camera_follow_target(mgc_camera_t *camera, const mgc_sprite_t *target) {

    int16_t dx, dy;
    int16_t next_follow_line;

    if ( ( camera == NULL ) ||
         ( target == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    if ( camera->x_enabled ) {
        mgc_follow_settings_t *settings = &camera->x_settings;
        if ( target->x < settings->start_line ) {
            next_follow_line = settings->start_line;
        } else if ( target->x > (settings->end_line + settings->deadzone) ) {
            next_follow_line = settings->end_line;
        } else {
            if ( target->x < camera->x_follow_line ) {
                next_follow_line = target->x;
            } else if ( target->x > (camera->x_follow_line + settings->deadzone) ) {
                next_follow_line = target->x - settings->deadzone;
            } else {
                next_follow_line = camera->x_follow_line;
            }
        }
        dx = next_follow_line - camera->x_follow_line;
        camera->x_follow_line = next_follow_line;
    } else {
        dx = 0;
    }

    if ( camera->y_enabled ) {
        mgc_follow_settings_t *settings = &camera->y_settings;
        if ( target->y < settings->start_line ) {
            next_follow_line = settings->start_line;
        } else if ( target->y > (settings->end_line + settings->deadzone) ) {
            next_follow_line = settings->end_line;
        } else {
            if ( target->y < camera->y_follow_line ) {
                next_follow_line = target->y;
            } else if ( target->y > (camera->y_follow_line + settings->deadzone) ) {
                next_follow_line = target->y - settings->deadzone;
            } else {
                next_follow_line = camera->y_follow_line;
            }
        }
        dy = next_follow_line - camera->y_follow_line;
        camera->y_follow_line = next_follow_line;
    } else {
        dy = 0;
    }

    camera->x += dx;
    camera->y += dy;
}

bool camera_get_position(const mgc_camera_t *camera, mgc_point_t *cam_pos) {
    if ( ( camera == NULL ) ||
         ( cam_pos == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    cam_pos->x = camera->x;
    cam_pos->y = camera->y;
    return true;
}

//////////////////////////////// Legacy ////////////////////////////////
void camera_update(mgc_pixelbuffer_t *pixelbuffer, mgc_camera_t *camera, const mgc_sprite_t *target) {

    if ( ( pixelbuffer == NULL ) ||
         ( camera == NULL ) ||
         ( target == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }

    camera_follow_target(camera, target);

    pixelbuffer_set_cell_offset(pixelbuffer, camera->x, camera->y);
}

