/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "mgc/mgc.h"
#include "system/system.h"
#include "resources/generates/tileset/tileset_balls.h"

#define NUM_OF_BALLS  7

static double rand_norm(void) {
    return (double)rand() / RAND_MAX;
}

int main(void) {

    mgc_pixelbuffer_t pixelbuffer;
    mgc_sprite_t balls[NUM_OF_BALLS];
    const mgc_display_if_t *display;
    uint16_t width, height;

    // Get the screen size.
    width = sys_get_display_width();
    height = sys_get_display_height();
    
    // Get display driver.
    display = sys_get_display_driver();

    // Initialize the display driver.
    display->init();

    // Initialize the pixel buffer.
    pixelbuffer_init(&pixelbuffer);

    // Sets the entire screen to black.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR_BLACK);
    for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
        for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    // Configure each sprite.
    for ( size_t idx = 0; idx < NUM_OF_BALLS; idx++ ) {

        mgc_sprite_t *sprite = &balls[idx];
        size_t tile_idx = idx;

        // Initialize the sprite.
        sprite_init(sprite, 0);

        // Set a tileset to a sprite. 
        sprite_set_tileset(sprite, &tileset_balls);

        // Specify the tile to be displayed as a sprite by its ID.
        sprite_set_tile_idx(sprite, tile_idx);

        // Set the sprite position.
        sprite_set_position(sprite, width*rand_norm(), height*rand_norm());
    }

    // Set the background color of the pixel buffer.
    pixelbuffer_set_back_color(&pixelbuffer, MGC_COLOR_BLACK);
    
    // Enable reflesh mode.
    pixelbuffer_set_refresh_mode(&pixelbuffer, true);

    // Draw the entire screen.
    for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
        for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
            for ( size_t idx = 0; idx < NUM_OF_BALLS; idx++ ) {
                mgc_sprite_t *sprite = &balls[idx];
                // Blend the sprite into the pixel buffer.
                sprite_apply_cell_blending(sprite, &pixelbuffer, cell_x, cell_y);
            }
            // Draw the content of the blended pixel buffer onto the display.
            // If the refresh mode of the pixel buffer is enabled, the content
            // of the pixel buffer is cleared and initialized to the background
            // color at the end of this function.
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    while (1);

    return 0;
}
