/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_MAP_H
#define MGC_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

enum mgc_map_compression {
    MGC_MAP_COMPRESSION_NONE,
    MGC_MAP_COMPRESSION_RUN_LENGTH,
};

typedef struct mgc_map {
    const uint8_t *map;
    const size_t cell_count;
    uint16_t map_width;
    uint16_t map_height;
    enum mgc_map_compression comp_type;
} mgc_map_t;

static inline uint8_t map_decompress_and_get_tile_id(const mgc_map_t *map, uint16_t i, uint16_t j) {
    if ( ( map == NULL ) ||
         ( map->map == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return 0;
    }
    if ( map->comp_type == MGC_MAP_COMPRESSION_NONE ) {
        return map->map[i+j*map->map_width];
    } else if ( map->comp_type == MGC_MAP_COMPRESSION_RUN_LENGTH ) {
        size_t sum_len;
        size_t target_len = i+j*map->map_width+1;
        size_t total_len = (size_t)(map->map_width)*map->map_height;
        if ( target_len < (total_len/2) ) {
            sum_len = 0;
            for ( size_t idx = 1; idx < map->cell_count; idx+=2 ) {
                sum_len += map->map[idx];
                if ( sum_len >= target_len ) {
                    return map->map[idx-1];
                }
            }
        } else {
            sum_len = total_len;
            for ( size_t idx = (map->cell_count-1); idx >= 0; idx-=2 ) {
                if ( sum_len < target_len ) {
                    return map->map[idx+1];
                }
                sum_len -= map->map[idx];
            }
        }
        MGC_WARN("Unexpected index");
        return 0;
    } else {
        MGC_WARN("Unexpected compression type");
        return 0;
    }
}

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_MAP_H*/
