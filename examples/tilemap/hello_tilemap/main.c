/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "mgc/mgc.h"
#include "system/system.h"
#include "resources/generates/tileset/tileset_map_elements.h"
#include "resources/generates/map/sample_map.h"

int main(void) {

    mgc_pixelbuffer_t pixelbuffer;
    mgc_tilemap_t tilemap;
    const mgc_display_if_t *display;
    uint16_t width, height;

    // Get the display driver.
    display = sys_get_display_driver();

    // Get the screen size.
    width = display->get_width();
    height = display->get_height();

    // Initialize the display driver.
    display->init();

    // Initialize the pixel buffer.
    pixelbuffer_init(&pixelbuffer);

    // Initialize the tilemap
    tilemap_init(&tilemap, 0, &sample_map, &tileset_map_elements);

    // Set the background color of the pixel buffer.
    pixelbuffer_set_back_color(&pixelbuffer, MGC_COLOR(0.0F, 1.0F, 1.0F));
    
    // Draw the entire screen.
    for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
        for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
            tilemap_apply_cell_blending(&tilemap, &pixelbuffer, cell_x, cell_y);
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    while (1);

    return 0;
}
