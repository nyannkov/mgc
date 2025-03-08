/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "tilemap.h"

void tilemap_init(mgc_tilemap_t *tilemap, mgc_id_t id, const mgc_map_t *map, const mgc_tileset_t *tileset) {
    if ( ( tilemap == NULL ) ||
         ( map == NULL ) ||
         ( tileset == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->id = id;
    tilemap->x = 0;
    tilemap->y = 0;
    tilemap->r_cell_x_ofs = 1;
    tilemap->r_cell_y_ofs = 1;
    tilemap->enabled = MGC_DEFAULT_ENABLED;
    tilemap->map = map;
    tilemap->tileset = tileset;
}

void tilemap_set_enabled(mgc_tilemap_t *tilemap, bool enabled) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->enabled = enabled;
}

void tilemap_set_position(mgc_tilemap_t *tilemap, int16_t x, int16_t y) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->x = x;
    tilemap->y = y;
}

void tilemap_set_r_cell_offset(mgc_tilemap_t *tilemap, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( tilemap == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    tilemap->r_cell_x_ofs = r_cell_x_ofs;
    tilemap->r_cell_y_ofs = r_cell_y_ofs;
}

bool tilemap_apply_cell_blending(const mgc_tilemap_t *tilemap, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {
    int16_t l0, l1;
    int16_t r0, r1;
    int16_t t0, t1;
    int16_t b0, b1;

    if ( ( tilemap == NULL ) ||
         ( pixelbuffer == NULL ) ||
         ( tilemap->map == NULL ) ||
         ( tilemap->tileset == NULL ) ||
         ( tilemap->tileset->tile_width  != MGC_CELL_LEN ) ||
         ( tilemap->tileset->tile_height != MGC_CELL_LEN )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( tilemap->enabled == false ) {
        MGC_INFO("Handler is disabled");
        return false;
    }
    l0 = tilemap->x;
    r0 = l0 + tilemap->map->map_width * MGC_CELL_LEN - 1;
    t0 = tilemap->y;
    b0 = t0 + tilemap->map->map_height * MGC_CELL_LEN - 1;

    l1 = cell_x;
    t1 = cell_y;
    if ( tilemap->r_cell_x_ofs != 0 ) {
        l1 += pixelbuffer->cell_x_ofs / tilemap->r_cell_x_ofs;
    }
    if ( tilemap->r_cell_y_ofs != 0 ) {
        t1 += pixelbuffer->cell_y_ofs / tilemap->r_cell_y_ofs;
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
        mgc_color_t *pixelbuf;

        tileset = tilemap->tileset;
        palette_array = tileset->palette_array;
        map = tilemap->map;
        pixelbuf = pixelbuffer->pixelbuf;

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
            tile_lt = map_decompress_and_get_tile_id(map, i, j)&0x7F;
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
            tile_rt = map_decompress_and_get_tile_id(map, i, j)&0x7F;
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
            tile_lb = map_decompress_and_get_tile_id(map, i, j)&0x7F;
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
            tile_rb = map_decompress_and_get_tile_id(map, i, j)&0x7F;
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
                            idx = MGC_GET_PIXELBUF_INDEX(x-x_s, y-y_s);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x-x_e+MGC_CELL_LEN-1, y-y_s);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x-x_s, y-y_e+MGC_CELL_LEN-1);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x-x_e+MGC_CELL_LEN-1, y-y_e+MGC_CELL_LEN-1);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x-x_s, y);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x-x_e+MGC_CELL_LEN-1, y);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x, y-y_s);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x, y-y_e+MGC_CELL_LEN-1);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
                            idx = MGC_GET_PIXELBUF_INDEX(x, y);
                            pixelbuf[idx] = MGC_COLOR_SWAP(palette_array[color_index]);
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
