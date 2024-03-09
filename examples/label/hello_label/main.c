/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "mgc/mgc.h"
#include "system/system.h"
#include "resources/generates/font/k8x12S.h"

int main(void) {

    mgc_pixelbuffer_t pixelbuffer;
    mgc_label_t label, label_big;
    const mgc_display_if_t *display;
    uint16_t width, height;

    // Getting the screen size.
    width = sys_get_display_width();
    height = sys_get_display_height();
    
    // Getting display driver.
    display = sys_get_display_driver();

    // Initialize display driver.
    display->init();

    // Initialize pixel buffer.
    pixelbuffer_init(&pixelbuffer);

    // Sets the entire screen to black.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR_BLACK);
    for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
        for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    // Initialize the label.
    label_init(&label, 0, &k8x12S, false);
    label_set_position(&label, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(8));
    label_set_size(&label, 8*10, 12);
    label_set_text(&label, "Hello!(通常の大きさ)");
    label_set_fore_color(&label, MGC_COLOR_WHITE);

    // Initialize the label with double the font width and height.
    label_init(&label_big, 0, &k8x12S, true);
    label_set_position(&label_big, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(10));
    label_set_size(&label_big, 8*2*11, 12*2);
    label_set_text(&label_big, "Hello!(幅・高さが二倍)");
    label_set_fore_color(&label_big, MGC_COLOR_BLACK);
    // Enable the background color of the label.
    label_set_enable_back_color(&label_big, true);
    label_set_back_color(&label_big, MGC_COLOR_WHITE);

    // Set the background color of the pixel buffer.
    pixelbuffer_set_back_color(&pixelbuffer, MGC_COLOR_BLACK);
    
    // Draw the entire screen.
    for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
        for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
            label_apply_cell_blending(&label, &pixelbuffer, cell_x, cell_y);
            label_apply_cell_blending(&label_big, &pixelbuffer, cell_x, cell_y);
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    while (1) {
    }
    return 0;
}
