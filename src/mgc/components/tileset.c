/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
 #include "tileset.h"


bool tileset_copy_palette(
    const mgc_tileset_t *tileset,
    mgc_color_t* dest_palette,
    size_t dest_count
) {

    if ( tileset == NULL ||
         tileset->palette_array == NULL 
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }

    if ( dest_palette == NULL ) {
        MGC_WARN("dest_palette is NULL");
        return false;
    }

    if ( dest_count < tileset->palette_count ) {
        MGC_WARN("dest_count is too small");
        return false;
    }

    for ( size_t i = 0; i < tileset->palette_count; i++ ) {
        dest_palette[i] = tileset->palette_array[i];
    }

    return true;
}

