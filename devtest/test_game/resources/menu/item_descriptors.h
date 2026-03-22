#ifndef MGC_ITEM_DESCRIPTORS_H
#define MGC_ITEM_DESCRIPTORS_H

#include "mgc/mgc.h"

typedef struct item_descriptor {
    const char *name;
    const char *description;
    size_t tile_idx;
} item_descriptor_t;

typedef struct item_descriptors {
    const char *title;
    const mgc_tileset_t* tileset;
    const item_descriptor_t* descriptors;
    size_t descriptor_count;
} item_descriptors_t;

#endif/*MGC_ITEM_DESCRIPTORS_H*/

