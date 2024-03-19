/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "mgc/mgc.h"
#include "system/system.h"
#include "resources/generates/tileset/tileset_balls.h"
#include "resources/generates/tileset/tileset_map_elements.h"
#include "resources/generates/map/sample_map.h"

#define TILE_IDX_BALL   2
#define HITBOX_ID       0
#define V_ABS           4

struct ball {
    mgc_sprite_t sprite;
    mgc_hitbox_t hitbox;
    int16_t vx;
    int16_t vy;
};

static void init_ball(struct ball *ball, mgc_id_t id, int16_t x, int16_t y) {

    // Set up the sprite.
    sprite_init(&ball->sprite, id);
    sprite_set_tileset(&ball->sprite, &tileset_balls);
    sprite_set_tile_idx(&ball->sprite, TILE_IDX_BALL);
    sprite_set_position(&ball->sprite, x, y);

    // Set up the hitbox and register it to the sprite.
    hitbox_init(&ball->hitbox, HITBOX_ID, 2, 2, 12, 12);
    sprite_set_hitbox_array(&ball->sprite, &ball->hitbox, 1);

    // Set velocities.
    ball->vx = V_ABS;
    ball->vy = V_ABS;
}

int main(void) {

    struct ball ball;
    mgc_tilemap_t tilemap;
    mgc_pixelbuffer_t pixelbuffer;
    const mgc_display_if_t *display;
    uint16_t width, height;
    mgc_maphit_t maphit;

    // Get the screen size.
    width = sys_get_display_width();
    height = sys_get_display_height();
    
    // Get the display driver.
    display = sys_get_display_driver();

    // Initialize the display driver.
    display->init();

    // Initialize the pixel buffer.
    pixelbuffer_init(&pixelbuffer);

    // Set the entire screen to black.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR_BLACK);
    for (int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1)) {
        for (int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1)) {
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    // Initialize a ball.
    init_ball(&ball, 0, MGC_CELL2PIXEL(4) + 1, MGC_CELL2PIXEL(2) + 2);

    // Initialize a tilemap.
    tilemap_init(&tilemap, 0, &sample_map, &tileset_map_elements);

    // Set the background color of the pixel buffer.
    pixelbuffer_set_back_color(&pixelbuffer, MGC_COLOR_BLACK);
    
    // Enable refresh mode.
    pixelbuffer_set_refresh_mode(&pixelbuffer, true);

    // Initialize the maphit.
    maphit_init(&maphit);

    while (1) {

        int16_t pushback_x = 0;
        int16_t pushback_y = 0;

        // Update the ball's position.
        ball.sprite.x += ball.vx;
        ball.sprite.y += ball.vy;

        // Set up to detect collisions between the block and the ball.
        maphit_setup_detection(&maphit, &ball.sprite, HITBOX_ID, &tilemap);

        // Continue as long as collisions are detected.
        while ( maphit_detect(&maphit) == true ) {
            // Write collision handling code here.
        }
        
        // Calcurate the distance pushed back from the wall.
        maphit_calc_wall_pushback(
            &maphit,
            &pushback_x,
            &pushback_y,
            MGC_CORNER_ESCAPE_TYPE_X_DIRECTION
        );

        // Reflect the pushback distance on the sprite's position.
        // Also, if the directions of pushback and velocity are opposite, 
        // reverse the velocity direction. 
        if ( ( (pushback_x > 0) && (ball.vx < 0) ) ||
             ( (pushback_x < 0) && (ball.vx > 0) )
        ) {
            ball.sprite.x += pushback_x;
            ball.vx *= -1;
        }

        if ( ( (pushback_y > 0) && (ball.vy < 0) ) ||
             ( (pushback_y < 0) && (ball.vy > 0) )
        ) {
            ball.sprite.y += pushback_y;
            ball.vy *= -1;
        }

        // Draw the entire screen.
        for (int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1)) {
            for (int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1)) {
                tilemap_apply_cell_blending(&tilemap, &pixelbuffer, cell_x, cell_y);
                sprite_apply_cell_blending(&ball.sprite, &pixelbuffer, cell_x, cell_y);

                // Send the contents of the pixel buffer to the display.
                pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
            }
        }
    }
    return 0;
}

