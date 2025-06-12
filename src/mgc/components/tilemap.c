/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "tilemap.h"

void tilemap_init(mgc_tilemap_t *tilemap, mgc_id_t id, const mgc_map_t *map, const mgc_tileset_t *tileset) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->id = id;
    tilemap->x = 0;
    tilemap->y = 0;
    tilemap->parallax_factor_x = 1.0F;
    tilemap->parallax_factor_y = 1.0F;
    tilemap->visible = MGC_DEFAULT_VISIBLE;
    tilemap->hit_enabled = true;
    tilemap->map = map;
    tilemap->tileset = tileset;
    tilemap->callbacks.context = NULL;
    tilemap->callbacks.on_get_tile_id = NULL;
}

void tilemap_set_id(mgc_tilemap_t *tilemap, mgc_id_t id) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->id = id;
}

void tilemap_set_map(mgc_tilemap_t *tilemap,const mgc_map_t *map) {
    if ( ( tilemap == NULL ) ||
         ( map == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->map = map;
}

void tilemap_set_tileset(mgc_tilemap_t *tilemap,const mgc_tileset_t *tileset) {
    if ( ( tilemap == NULL ) ||
         ( tileset == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->tileset = tileset;
}

void tilemap_set_visible(mgc_tilemap_t *tilemap, bool v) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->visible = v;
}

void tilemap_set_hit_enabled(mgc_tilemap_t *tilemap, bool enabled) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->hit_enabled = enabled;
}

void tilemap_set_hit_visible(mgc_tilemap_t *tilemap, bool v) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->visible = v;
}

void tilemap_set_position(mgc_tilemap_t *tilemap, int16_t x, int16_t y) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->x = x;
    tilemap->y = y;
}

void tilemap_set_parallax_factor(mgc_tilemap_t *tilemap, float factor_x, float factor_y) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->parallax_factor_x = factor_x;
    tilemap->parallax_factor_y = factor_y;
}

void tilemap_set_callbacks(mgc_tilemap_t *tilemap, const mgc_tilemap_callbacks_t *callbacks) {
    if ( ( tilemap == NULL ) ||
         ( callbacks == NULL ) 
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->callbacks.context = callbacks->context;
    tilemap->callbacks.on_get_tile_id = callbacks->on_get_tile_id;
}

void tilemap_set_on_get_tile_id_cb(mgc_tilemap_t *tilemap, uint8_t (*cb)(uint8_t tile_id, uint16_t row, uint16_t col, void *context), void *context) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->callbacks.context = context;
    tilemap->callbacks.on_get_tile_id = cb;
}

bool tilemap_draw(const mgc_tilemap_t *tilemap, mgc_framebuffer_t *fb, const mgc_point_t *cam_pos, const mgc_draw_options_t *options) {
    if ( fb == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    return tilemap_draw_raw(tilemap, fb->buffer, fb->width, fb->height, cam_pos, options);
}

bool tilemap_draw_cell(
        const mgc_tilemap_t *tilemap,
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
    return tilemap_draw_cell_raw(tilemap, pb->pixelbuf, cell_x, cell_y, cam_pos, options);
}

bool tilemap_draw_raw(
        const mgc_tilemap_t *tilemap,
        mgc_color_t *buffer,
        uint16_t width,
        uint16_t height,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    // 0: tilemap, 1: camera
    int16_t l0, l1;
    int16_t r0, r1;
    int16_t t0, t1;
    int16_t b0, b1;

    if ( ( tilemap == NULL ) ||
         ( tilemap->map == NULL ) ||
         ( tilemap->tileset == NULL ) ||
         ( tilemap->tileset->tile_width  != MGC_CELL_LEN ) ||
         ( tilemap->tileset->tile_height != MGC_CELL_LEN ) ||
         ( buffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( tilemap->visible == false ) {
        MGC_INFO("Handler is not visible");
        return false;
    }

    (void)options;

    l1 = 0;
    t1 = 0;

    if ( cam_pos != NULL ) {
        l1 += MGC_PARALLAX_SHIFT(cam_pos->x, tilemap->parallax_factor_x);
        t1 += MGC_PARALLAX_SHIFT(cam_pos->y, tilemap->parallax_factor_y);
    }
    r1 = l1 + width - 1;
    b1 = t1 + height - 1;


    l0 = tilemap->x;
    t0 = tilemap->y;
    r0 = l0 + tilemap->map->map_width * MGC_CELL_LEN - 1;
    b0 = t0 + tilemap->map->map_height * MGC_CELL_LEN - 1;

    if ( (l0<=r1) && (l1<=r0) && (t0<=b1) && (t1<=b0) ) {
        const uint8_t tile_count = tilemap->tileset->tile_count;
        const mgc_color_t *palette_array = tilemap->tileset->palette_array;
        const uint8_t **tile_array = tilemap->tileset->tile_array;
        const mgc_map_t *map = tilemap->map;
        uint16_t i_s, j_s, i_e, j_e;
        i_s = (l1 <= l0) ? 0 : MGC_DIV_CELL_LEN(l1-l0);
        j_s = (t1 <= t0) ? 0 : MGC_DIV_CELL_LEN(t1-t0);
        i_e = (r0 <= r1) ? (map->map_width-1) : MGC_DIV_CELL_LEN(r1-l0);
        j_e = (b0 <= b1) ? (map->map_height-1) : MGC_DIV_CELL_LEN(b1-t0);

        for ( uint16_t i = i_s; i <= i_e; i++ ) {
            for ( uint16_t j = j_s; j <= j_e; j++ ) {
                uint8_t tile_id = map_decompress_and_get_tile_id(map, i, j);
                if ( tilemap->callbacks.on_get_tile_id != NULL ) {
                    tile_id = tilemap->callbacks.on_get_tile_id(tile_id, j, i, tilemap->callbacks.context);
                }
                tile_id &= 0x7F;
                if ( (0 < tile_id ) && ( tile_id < tile_count ) ) {
                    const uint8_t *tile = tile_array[tile_id];
                    int16_t l2 = l0 + (i * MGC_CELL_LEN);
                    int16_t r2 = l2 + MGC_CELL_LEN - 1;
                    int16_t t2 = t0 + (j * MGC_CELL_LEN);
                    int16_t b2 = t2 + MGC_CELL_LEN - 1;
                    int16_t x_s = (( l1 < l2 ) ? l2 : l1) - l2;
                    int16_t x_e = (( r1 < r2 ) ? r1 : r2) - l2;
                    int16_t y_s = (( t1 < t2 ) ? t2 : t1) - t2;
                    int16_t y_e = (( b1 < b2 ) ? b1 : b2) - t2;
                    int16_t x, y;
                    int32_t wy;
                    for ( x = x_s; x <= x_e; x++ ) {
                        for ( y = y_s, wy = y_s*MGC_CELL_LEN; y <= y_e; y++, wy+=MGC_CELL_LEN ) {
                            int16_t color_index = tile[x + wy];
                            if ( color_index != 0 ) {
                                size_t idx = MGC_GET_PIXELBUF_INDEX(x+l2-l1, y+t2-t1, width, height);
                                buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                            }
                        }
                    }
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool tilemap_draw_cell_raw(
        const mgc_tilemap_t *tilemap,
        mgc_color_t  *cell_buffer,
        int16_t cell_x,
        int16_t cell_y,
        const mgc_point_t *cam_pos,
        const mgc_draw_options_t *options
) {
    int16_t l0, l1;
    int16_t r0, r1;
    int16_t t0, t1;
    int16_t b0, b1;

    if ( ( tilemap == NULL ) ||
         ( cell_buffer == NULL ) ||
         ( tilemap->map == NULL ) ||
         ( tilemap->tileset == NULL ) ||
         ( tilemap->tileset->tile_width  != MGC_CELL_LEN ) ||
         ( tilemap->tileset->tile_height != MGC_CELL_LEN )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( tilemap->visible == false ) {
        MGC_INFO("Handler is not visible");
        return false;
    }
    l0 = tilemap->x;
    r0 = l0 + tilemap->map->map_width * MGC_CELL_LEN - 1;
    t0 = tilemap->y;
    b0 = t0 + tilemap->map->map_height * MGC_CELL_LEN - 1;

    l1 = cell_x;
    t1 = cell_y;
    if ( cam_pos != NULL ) {
        l1 += MGC_PARALLAX_SHIFT(cam_pos->x, tilemap->parallax_factor_x);
        t1 += MGC_PARALLAX_SHIFT(cam_pos->y, tilemap->parallax_factor_y);
    }
    r1 = l1 + MGC_CELL_LEN - 1;
    b1 = t1 + MGC_CELL_LEN - 1;

    if ( (l0<=r1) && (l1<=r0) && (t0<=b1) && (t1<=b0) ) {
        int16_t i, j;
        uint8_t tile_lt, tile_rt, tile_lb, tile_rb;
        const mgc_tileset_t *tileset;
        const mgc_map_t *map;
        const uint8_t *tile;
        const mgc_color_t *palette_array; 
        int16_t x, y, wy;
        int16_t x_s, x_e, y_s, y_e;
        int16_t color_index;

        tileset = tilemap->tileset;
        palette_array = tileset->palette_array;
        map = tilemap->map;

        /* (left, top) */
        i = l1 - tilemap->x;
        j = t1 - tilemap->y;
        if ( (0 <= i) &&
             (i < MGC_CELL2PIXEL(map->map_width)) &&
             (0 <= j) &&
             (j < MGC_CELL2PIXEL(map->map_height))
        ) {
            i = MGC_DIV_CELL_LEN(i);
            j = MGC_DIV_CELL_LEN(j);
            tile_lt = map_decompress_and_get_tile_id(map, i, j);
            if ( tilemap->callbacks.on_get_tile_id != NULL ) {
                tile_lt = tilemap->callbacks.on_get_tile_id(tile_lt, j, i, tilemap->callbacks.context);
            }
            tile_lt &= 0x7F;
            if ( tile_lt >= tileset->tile_count ) tile_lt = 0;
        } else {
            tile_lt = 0;
        }

        /* (right, top) */
        i = r1 - tilemap->x;
        j = t1 - tilemap->y;
        if ( (0 <= i) &&
             (i < MGC_CELL2PIXEL(map->map_width)) &&
             (0 <= j) &&
             (j < MGC_CELL2PIXEL(map->map_height))
        ) {
            i = MGC_DIV_CELL_LEN(i);
            j = MGC_DIV_CELL_LEN(j);
            tile_rt = map_decompress_and_get_tile_id(map, i, j);
            if ( tilemap->callbacks.on_get_tile_id != NULL ) {
                tile_rt = tilemap->callbacks.on_get_tile_id(tile_rt, j, i, tilemap->callbacks.context);
            }
            tile_rt &= 0x7F;
            if ( tile_rt >= tileset->tile_count ) tile_rt = 0;
        } else {
            tile_rt = 0;
        }

        /* (left, bottom) */
        i = l1 - tilemap->x;
        j = b1 - tilemap->y;
        if ( (0 <= i) &&
             (i < MGC_CELL2PIXEL(map->map_width)) &&
             (0 <= j) &&
             (j < MGC_CELL2PIXEL(map->map_height))
        ) {
            i = MGC_DIV_CELL_LEN(i);
            j = MGC_DIV_CELL_LEN(j);
            tile_lb = map_decompress_and_get_tile_id(map, i, j);
            if ( tilemap->callbacks.on_get_tile_id != NULL ) {
                tile_lb = tilemap->callbacks.on_get_tile_id(tile_lb, j, i, tilemap->callbacks.context);
            }
            tile_lb &= 0x7F;
            if ( tile_lb >= tileset->tile_count ) tile_lb = 0;
        } else {
            tile_lb = 0;
        }

        /* (right, bottom) */
        i = r1 - tilemap->x;
        j = b1 - tilemap->y;
        if ( (0 <= i) &&
             (i < MGC_CELL2PIXEL(map->map_width)) &&
             (0 <= j) &&
             (j < MGC_CELL2PIXEL(map->map_height))
        ) {
            i = MGC_DIV_CELL_LEN(i);
            j = MGC_DIV_CELL_LEN(j);
            tile_rb = map_decompress_and_get_tile_id(map, i, j);
            if ( tilemap->callbacks.on_get_tile_id != NULL ) {
                tile_rb = tilemap->callbacks.on_get_tile_id(tile_rb, j, i, tilemap->callbacks.context);
            }
            tile_rb &= 0x7F;
            if ( tile_rb >= tileset->tile_count ) tile_rb = 0;
        } else {
            tile_rb = 0;
        }

        if ( (tile_lt == 0 ) &&
             (tile_rt == 0 ) &&
             (tile_lb == 0 ) &&
             (tile_rb == 0 )
        ) {
            return false;
        }

        x_s = MGC_MOD_CELL_LEN(l1 - tilemap->x);
        y_s = MGC_MOD_CELL_LEN(t1 - tilemap->y);
        x_e = MGC_MOD_CELL_LEN(r1 - tilemap->x);
        y_e = MGC_MOD_CELL_LEN(b1 - tilemap->y);

        if ( (x_s != 0) && (y_s != 0) ) {
            if ( tile_lt != 0 ) {
                tile = tileset->tile_array[tile_lt];
                for ( x = x_s; x < MGC_CELL_LEN; x++ ) {
                    for ( y = y_s, wy = y_s*MGC_CELL_LEN; y < MGC_CELL_LEN; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x-x_s, y-y_s);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
            if ( tile_rt != 0 ) {
                tile = tileset->tile_array[tile_rt];
                for ( x = 0; x <= x_e; x++ ) {
                    for ( y = y_s, wy = y_s*MGC_CELL_LEN; y < MGC_CELL_LEN; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x-x_e+MGC_CELL_LEN-1, y-y_s);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
            if ( tile_lb != 0 ) {
                tile = tileset->tile_array[tile_lb];
                for ( x = x_s; x < MGC_CELL_LEN; x++ ) {
                    for ( y = 0, wy = 0; y <= y_e; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x-x_s, y-y_e+MGC_CELL_LEN-1);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
            if ( tile_rb != 0 ) {
                tile = tileset->tile_array[tile_rb];
                for ( x = 0; x <= x_e; x++ ) {
                    for ( y = 0, wy = 0; y <= y_e; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x-x_e+MGC_CELL_LEN-1, y-y_e+MGC_CELL_LEN-1);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
        } else if ( (x_s != 0) && (y_s == 0) ) {
            if ( tile_lt != 0 ) {
                tile = tileset->tile_array[tile_lt];
                for ( x = x_s; x < MGC_CELL_LEN; x++ ) {
                    for ( y = 0, wy = 0; y < MGC_CELL_LEN; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x-x_s, y);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
            if ( tile_rt != 0 ) {
                tile = tileset->tile_array[tile_rt];
                for ( x = 0; x <= x_e; x++ ) {
                    for ( y = 0, wy = 0; y < MGC_CELL_LEN; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x-x_e+MGC_CELL_LEN-1, y);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
        } else if ( (x_s == 0) && (y_s != 0) ) {
            if ( tile_lt != 0 ) {
                tile = tileset->tile_array[tile_lt];
                for ( x = 0; x < MGC_CELL_LEN; x++ ) {
                    for ( y = y_s, wy = y_s*MGC_CELL_LEN; y < MGC_CELL_LEN; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x, y-y_s);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
            if ( tile_lb != 0 ) {
                tile = tileset->tile_array[tile_lb];
                for ( x = 0; x < MGC_CELL_LEN; x++ ) {
                    for ( y = 0, wy = 0; y <= y_e; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x, y-y_e+MGC_CELL_LEN-1);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
        } else {
            if ( tile_lt != 0 ) {
                tile = tileset->tile_array[tile_lt];
                for ( x = 0; x < MGC_CELL_LEN; x++ ) {
                    for ( y = 0, wy = 0; y < MGC_CELL_LEN; y++, wy+=MGC_CELL_LEN ) {
                        color_index = tile[x+wy];
                        if ( color_index != 0 ) {
                            size_t idx;
                            idx = MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x, y);
                            cell_buffer[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
                        }
                    }
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

//////////////////////////////// Legacy ////////////////////////////////
bool tilemap_apply_cell_blending(const mgc_tilemap_t *tilemap, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {

    if ( pixelbuffer == NULL ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    mgc_point_t cam_pos = {pixelbuffer->cell_x_ofs, pixelbuffer->cell_y_ofs};

    return tilemap_draw_cell(tilemap, pixelbuffer, cell_x, cell_y, &cam_pos, NULL);
}

void tilemap_set_r_cell_offset(mgc_tilemap_t *tilemap, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    tilemap->parallax_factor_x = (r_cell_x_ofs != 0) ? (1.0F / r_cell_x_ofs) : 0.0F;
    tilemap->parallax_factor_y = (r_cell_y_ofs != 0) ? (1.0F / r_cell_y_ofs) : 0.0F;
}

