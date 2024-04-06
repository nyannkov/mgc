/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "mgc/mgc.h"
#include "system/system.h"

int main(void) {

    mgc_pixelbuffer_t pixelbuffer;
    const mgc_display_if_t *display;
    uint16_t width, height;

    // Get display driver.
    display = sys_get_display_driver();

    // Get the screen size.
    width = display->get_width();
    height = display->get_height();

    // Initialize the display driver.
    display->init();

    // Initialize the pixelbuffer.
    pixelbuffer_init(&pixelbuffer);

    // Sets the entire screen to black.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR_BLACK);
    for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
        for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    // Paint the cell at (row=0, colums=0) black.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(0.0F, 0.0F, 0.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0));

    // Paint the cell at (row=0, colums=1) red.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(1.0F, 0.0F, 0.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(0));
    
    // Paint the cell at (row=0, colums=2) green.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(0.0F, 1.0F, 0.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(0));

    // Paint the cell at (row=0, colums=3) blue.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(0.0F, 0.0F, 1.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(0));

    // Paint the cell at (row=1, colums=0) yellow.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(1.0F, 1.0F, 0.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(1));
    
    // Paint the cell at (row=1, colums=1) cyan.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(0.0F, 1.0F, 1.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(1));

    // Paint the cell at (row=1, colums=2) magenta.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(1.0F, 0.0F, 1.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(1));

    // Paint the cell at (row=1, colums=3) white.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR(1.0F, 1.0F, 1.0F));
    pixelbuffer_draw_cell(&pixelbuffer, display, MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(1));

    while (1);

    return 0;
}
