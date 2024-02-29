/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <stdlib.h>
#include "mgc/mgc.h"
#include "system/system.h"
#include "resources/generates/tileset/tileset_balls_32x32.h"

#define NUM_OF_BALLS        10 

#define TILE_IDX_NOT_HIT    1
#define TILE_IDX_HIT        2

#define HITBOX_ID           0

struct ball {
    mgc_sprite_t sprite;
    mgc_hitbox_t hitbox;
    int16_t vx;
    int16_t vy;
};

static void init_ball(struct ball *ball, mgc_id_t id, int16_t x, int16_t y) {
    // Set up the sprite.
    sprite_init(&ball->sprite, id);
    sprite_set_tileset(&ball->sprite, &tileset_balls_32x32);
    sprite_set_tile_idx(&ball->sprite, TILE_IDX_NOT_HIT);
    sprite_set_position(&ball->sprite, x, y);

    // Set up the hitbox and register it to the sprite.
    hitbox_init(&ball->hitbox, HITBOX_ID, 3, 3, 26, 26);
    sprite_set_hitbox_array(&ball->sprite, &ball->hitbox, 1);

    // Set velocities.
    ball->vx = 3;
    ball->vy = 3;
}

static double rand_norm(void) {
    return (double)rand() / RAND_MAX;
}

int main(void) {
    static struct ball balls[NUM_OF_BALLS];
    mgc_pixelbuffer_t pixelbuffer;
    const mgc_display_if_t *display;
    uint16_t width, height;
    mgc_sprhit_t sprhit;

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

    // Configure each ball.
    for (size_t idx = 0; idx < NUM_OF_BALLS; idx++) {
        init_ball(&balls[idx], idx, width * rand_norm(), height * rand_norm());
    }

    // Set the background color of the pixel buffer.
    pixelbuffer_set_back_color(&pixelbuffer, MGC_COLOR_BLACK);
    
    // Enable refresh mode.
    pixelbuffer_set_refresh_mode(&pixelbuffer, true);

    // Initialize sprhit.
    sprhit_init(&sprhit);

    while (1) {
        // Update each ball's information.
        for (size_t idx = 0; idx < NUM_OF_BALLS; idx++) {
            struct ball *ball = &balls[idx];
            // Update the ball's position.
            ball->sprite.x += ball->vx;
            ball->sprite.y += ball->vy;

            // Boundary limitation.
            if (((ball->vx > 0) && ((ball->sprite.x + ball->hitbox.x0_ofs + ball->hitbox.width) >= width)) ||
                ((ball->vx < 0) && ((ball->sprite.x + ball->hitbox.x0_ofs) <= 0))) {
                ball->vx *= -1;
            }
            if (((ball->vy > 0) && ((ball->sprite.y + ball->hitbox.y0_ofs + ball->hitbox.height) >= height)) ||
                ((ball->vy < 0) && ((ball->sprite.y + ball->hitbox.y0_ofs) <= 0))) {
                ball->vy *= -1;
            }

            // Set the tile index to represent the state where there is no collision with other sprites.
            sprite_set_tile_idx(&ball->sprite, TILE_IDX_NOT_HIT);
        }
        
        // Check collision.
        for (size_t i = 0; i < NUM_OF_BALLS; i++) {
            for (size_t j = i + 1; j < NUM_OF_BALLS; j++) {
                // Set up sprhit to enable detection of collision state with sprites.
                sprhit_setup_detection(&sprhit, &balls[i].sprite, HITBOX_ID, &balls[j].sprite);
                if (sprhit_detect(&sprhit) == true) {
                    // Set the tile index to represent collision state with other sprites.
                    sprite_set_tile_idx(&balls[i].sprite, TILE_IDX_HIT);
                    sprite_set_tile_idx(&balls[j].sprite, TILE_IDX_HIT);
                }
            }
        }

        // Draw the entire screen.
        for (int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1)) {
            for (int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1)) {
                for (size_t idx = 0; idx < NUM_OF_BALLS; idx++) {
                    mgc_sprite_t *sprite = &balls[idx].sprite;
                    // Blend the sprite into the pixel buffer.
                    sprite_apply_cell_blending(sprite, &pixelbuffer, cell_x, cell_y);
                }
                // Send the contents of the pixel buffer to the display.
                pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
            }
        }
    }
    return 0;
}

