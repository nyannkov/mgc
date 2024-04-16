#include "display_lcd.h"

#if MGC_DISPLAY_LCD_TYPE==1

#include "ili9341/display_lcd_ili9341.h"
#define GET_INSTANCE display_lcd_ili9341_get_instance

#else
#error "No lcd-display selected."

#endif

const mgc_display_lcd_if_t *display_lcd_get_instance(void) {
    return GET_INSTANCE();
}

