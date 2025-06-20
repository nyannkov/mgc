/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "sprite.h"

void sprite_init(mgc_sprite_t *sprite, mgc_id_t id) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->id = id;
    sprite->x = 0;
    sprite->y = 0;
    sprite->parallax_factor_x = 1.0F;
    sprite->parallax_factor_y = 1.0F;
    sprite->visible = MGC_DEFAULT_VISIBLE;
    sprite->tileset = NULL;
    sprite->tile_idx = 0;
    sprite->hitbox_array = NULL;
    sprite->hitbox_count = 0;
    sprite->trim_left = 0;
    sprite->trim_right = 0;
    sprite->trim_top = 0;
    sprite->trim_bottom = 0;
}

void sprite_set_id(mgc_sprite_t *sprite, mgc_id_t id) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->id = id;
}

void sprite_set_visible(mgc_sprite_t *sprite, bool v) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->visible = v;
}

void sprite_set_position(mgc_sprite_t *sprite, int16_t x, int16_t y) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->x = x;
    sprite->y = y;
}

void sprite_set_tileset(struct mgc_sprite *sprite, const mgc_tileset_t *tileset) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->tileset = tileset;
    sprite->tile_idx = 0;
}

void sprite_set_tile_idx(mgc_sprite_t *sprite, size_t tile_idx) {
    if ( ( sprite == NULL ) ||
         ( sprite->tileset == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    if ( tile_idx >= sprite->tileset->tile_count ) {
        MGC_WARN("Out of index");
        return;
    }
    sprite->tile_idx = tile_idx;
}

void sprite_set_hitbox_array(mgc_sprite_t *sprite, const mgc_hitbox_t *hitbox_array, size_t hitbox_count) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->hitbox_array = hitbox_array;
    sprite->hitbox_count = hitbox_count;
}

void sprite_set_parallax_factor(mgc_sprite_t *sprite, float factor_x, float factor_y) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->parallax_factor_x = factor_x;
    sprite->parallax_factor_y = factor_y;
}

void sprite_set_trim(mgc_sprite_t *sprite, uint16_t left, uint16_t right, uint16_t top, uint16_t bottom) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    sprite->trim_left = left;
    sprite->trim_right = right;
    sprite->trim_top = top;
    sprite->trim_bottom = bottom;
}

static inline bool draw_buffer(
        const mgc_sprite_t *sprite,
        mgc_color_t *draw_buf,
        uint16_t buf_width,
        uint16_t buf_height,
        const mgc_point_t *cam_pos,
        const mgc_point_t *fov_ofs,
        const mgc_draw_options_t *options
) {
    // 0: sprite, 1: camera
    int16_t l0, l1;
    int16_t r0, r1;
    int16_t t0, t1;
    int16_t b0, b1;

    if ( ( sprite == NULL ) ||
         ( sprite->tileset == NULL ) ||
         ( sprite->tileset->tile_count == 0 ) ||
         ( draw_buf == NULL )
    ) {
        MGC_WARN("Invalid handler")
        return false;
    }
    if ( sprite->visible == false ) {
        MGC_INFO("Handler is not visible")
        return false;
    }

    (void)options;

    l0 = sprite->x;
    r0 = l0 + sprite->tileset->tile_width - 1;
    t0 = sprite->y;
    b0 = t0 + sprite->tileset->tile_height - 1;

    l0 += sprite->trim_left;
    r0 -= sprite->trim_right;
    t0 += sprite->trim_top;
    b0 -= sprite->trim_bottom;
    if ( (r0 <= l0) || (b0 <= t0) ) {
        return false;
    }

    if ( fov_ofs != NULL ) {
        l1 = fov_ofs->x;
        t1 = fov_ofs->y;
    } else {
        l1 = 0;
        t1 = 0;
    }

    if ( cam_pos != NULL ) {
        l1 += MGC_PARALLAX_SHIFT(cam_pos->x, sprite->parallax_factor_x);
        t1 += MGC_PARALLAX_SHIFT(cam_pos->y, sprite->parallax_factor_y);
    }
    r1 = l1 + buf_width - 1;
    b1 = t1 + buf_height - 1;

    if ( (l0<=r1) && (l1<=r0) && (t0<=b1) && (t1<=b0) ) {
        int16_t x, y;
        int32_t wy;
        int16_t x_s, y_s, x_e, y_e;
        mgc_color_t color;
        int16_t color_index;
        const uint8_t *tile;
        const mgc_color_t *palette_array;
        int16_t tile_width;

        tile_width = sprite->tileset->tile_width;
        tile = sprite->tileset->tile_array[sprite->tile_idx];
        tile = &tile[sprite->trim_left + sprite->trim_top * tile_width];
        palette_array = sprite->tileset->palette_array;

        x_s = (( l1 < l0 ) ? l0 : l1) - l0;
        x_e = (( r1 < r0 ) ? r1 : r0) - l0;
        y_s = (( t1 < t0 ) ? t0 : t1) - t0;
        y_e = (( b1 < b0 ) ? b1 : b0) - t0;

        for ( x = x_s; x <= x_e; x++ ) {
            for ( y = y_s, wy = y_s*tile_width; y <= y_e; y++, wy+=tile_width ) {
                color_index = tile[(int32_t)x+wy];
                if ( color_index != 0 ) {
                    size_t idx = MGC_GET_PIXELBUF_INDEX(x+l0-l1, y+t0-t1, buf_width, buf_height);
                    color = palette_array[color_index];
                    draw_buf[idx] = MGC_COLOR_SWAP(color);
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool sprite_draw(const mgc_sprite_t *sprite, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options) {

    if ( (fb == NULL) || (fb->buffer == NULL) ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    return sprite_draw_raw(sprite, fb->buffer, fb->width, fb->height, cam_pos, options);
}

bool sprite_draw_cell(
        const mgc_sprite_t *sprite,
        mgc_pixelbuffer_t *pb,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    if ( pb == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    return sprite_draw_cell_raw(sprite, pb->pixelbuf, cell_x, cell_y, cam_pos, options);
}

bool sprite_draw_raw(
        const mgc_sprite_t *sprite,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    mgc_point_t fov_ofs = {0, 0};

    return draw_buffer(sprite, buffer, width, height, cam_pos, &fov_ofs, options);
}

bool sprite_draw_cell_raw(
        const mgc_sprite_t *sprite,
        mgc_color_t *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    mgc_point_t fov_ofs = {cell_x, cell_y};

    return draw_buffer(sprite, cell_buffer, MGC_CELL_LEN, MGC_CELL_LEN, cam_pos, &fov_ofs, options);
}

//////////////////////////////// Legacy ////////////////////////////////
bool sprite_apply_cell_blending(const mgc_sprite_t *sprite, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {

    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    mgc_point_t cam_pos = {pixelbuffer->cell_x_ofs, pixelbuffer->cell_y_ofs};

    return sprite_draw_cell(sprite, pixelbuffer, cell_x, cell_y, &cam_pos, NULL);
}

void sprite_set_r_cell_offset(mgc_sprite_t *sprite, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( sprite == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    sprite->parallax_factor_x = (r_cell_x_ofs != 0) ? (1.0F / r_cell_x_ofs) : 0.0F;
    sprite->parallax_factor_y = (r_cell_y_ofs != 0) ? (1.0F / r_cell_y_ofs) : 0.0F;
}

