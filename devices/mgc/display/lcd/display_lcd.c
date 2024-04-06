#include "display_lcd.h"

#if MGC_DISPLAY_LCD_TYPE==1

#include "ili9341/display_lcd_ili9341.h"
const mgc_display_if_t *( * const display_lcd_get_instance )(void)
    = display_lcd_ili9341_get_instance;

#else
#error "No lcd-display selected."

#endif

