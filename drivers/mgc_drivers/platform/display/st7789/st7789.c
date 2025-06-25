#include "st7789.h"
#include "ports/st7789_port.h"

#define LCD_WIDTH                   MGC_DRIVERS_ST7789_WIDTH
#define LCD_HEIGHT                  MGC_DRIVERS_ST7789_HEIGHT

#define LCD_RST_PIN                 MGC_DRIVERS_ST7789_PORT__LCD_RST_PIN
#define LCD_DC_PIN                  MGC_DRIVERS_ST7789_PORT__LCD_DC_PIN
#define LCD_BL_PIN                  MGC_DRIVERS_ST7789_PORT__LCD_BL_PIN
#define LCD_CS_PIN                  MGC_DRIVERS_ST7789_PORT__LCD_CS_PIN

#define gpio_output_init            st7789_port__gpio_output_init
#define gpio_write                  st7789_port__gpio_write
#define spi_init                    st7789_port__spi_init
#define spi_transfer_blocking       st7789_port__spi_transfer_blocking
#define spi_transfer_async          st7789_port__spi_transfer_async
#define spi_get_transfer_state      st7789_port__spi_get_transfer_state
#define sleep_ms                    st7789_port__sleep_ms

#define ST7789_COMMAND_SLPIN   (0x10)   // Sleep In
#define ST7789_COMMAND_SLPOUT  (0x11)   // Sleep Out
#define ST7789_COMMAND_INVOFF  (0x20)   // Display Inversion Off  
#define ST7789_COMMAND_INVON   (0x21)   // Display Inversion On
#define ST7789_COMMAND_DISPOFF (0x28)   // Display Off
#define ST7789_COMMAND_DISPON  (0x29)   // Display On
#define ST7789_COMMAND_CASET   (0x2A)   // Column Address Set
#define ST7789_COMMAND_RASET   (0x2B)   // Column Address Set
#define ST7789_COMMAND_RAMWR   (0x2C)   // Memory Write

#define ST7789_COMMAND_MADCTL  (0x36)   // Memory Data Access Control
#define ST7789_MADCTL_MY   (1 << 7)     // D7 - Page Address Order (Row Address Order)
                                        // 0 = Top to Bottom, 1 = Bottom to Top
#define ST7789_MADCTL_MX   (1 << 6)     // D6 - Column Address Order
                                        // 0 = Left to Right, 1 = Right to Left
#define ST7789_MADCTL_MV   (1 << 5)     // D5 - Page/Column Exchange
                                        // 0 = Normal mode, 1 = X and Y exchanged
#define ST7789_MADCTL_ML   (1 << 4)     // D4 - Line Address Order
                                        // 0 = Refresh Top to Bottom, 1 = Bottom to Top
#define ST7789_MADCTL_BGR  (1 << 3)     // D3 - RGB/BGR Order
                                        // 0 = RGB, 1 = BGR
#define ST7789_MADCTL_MH   (1 << 2)     // D2 - Display Data Latch Order
                                        // 0 = Left to Right, 1 = Right to Left
#define ST7789_MADCTL_RESERVED (0x03)   // Reserved bits: D1 and D0 are usually unused


#define ST7789_COMMAND_COLMOD  (0x3A)   // Interface Pixel Format
#define ST7789_COLMOD_RGBIF_SHIFT 4     // D6-D4
#define ST7789_COLMOD_IFPF_SHIFT 0      // D2-D0


enum st7789_orientation {
    ST7789_ORIENTATION_PORTRAIT = 0,
    ST7789_ORIENTATION_LANDSCAPE
};

typedef enum {
    ST7789_PIXFMT_IF_12BIT = (0x03 << ST7789_COLMOD_IFPF_SHIFT),
    ST7789_PIXFMT_IF_16BIT = (0x05 << ST7789_COLMOD_IFPF_SHIFT),
    ST7789_PIXFMT_IF_18BIT = (0x06 << ST7789_COLMOD_IFPF_SHIFT),
    ST7789_PIXFMT_IF_24BIT = (0x07 << ST7789_COLMOD_IFPF_SHIFT) // 16M colors (truncated)
} st7789_pixfmt_if_t;

typedef enum {
    ST7789_PIXFMT_RGBIF_NONE  = (0x00 << ST7789_COLMOD_RGBIF_SHIFT), // default / not used
    ST7789_PIXFMT_RGBIF_65K   = (0x05 << ST7789_COLMOD_RGBIF_SHIFT), // 65K (R:G:B = 5:6:5)
    ST7789_PIXFMT_RGBIF_262K  = (0x06 << ST7789_COLMOD_RGBIF_SHIFT)  // 262K (6bit per channel)
} st7789_pixfmt_rgbif_t;

typedef struct st7789_registers {
    uint8_t MADCTRL;
    uint8_t COLMOD;
} st7789_registers_t;

static st7789_registers_t st7789_regs;

static inline uint8_t make_madctl(enum st7789_orientation orientation, bool use_bgr) {
    uint8_t madctl = 0;

    if (use_bgr) {
        madctl |= ST7789_MADCTL_BGR;
    }

    switch (orientation) {
    case ST7789_ORIENTATION_LANDSCAPE:
        madctl |= ST7789_MADCTL_MX | ST7789_MADCTL_MV;
        break;

    case ST7789_ORIENTATION_PORTRAIT: /* fall-through */
    default:
        break;
    }

    return madctl;
}

static inline uint8_t make_colmod(st7789_pixfmt_if_t ifpf, st7789_pixfmt_rgbif_t rgbif) {
    return (uint8_t)(ifpf | rgbif);
}

static void init_st7789_registers(st7789_registers_t *regs) {
    if ( regs == NULL ) {
        return;
    }

    regs->MADCTRL = make_madctl(ST7789_ORIENTATION_LANDSCAPE, false);
    regs->COLMOD = make_colmod(ST7789_PIXFMT_IF_16BIT, ST7789_PIXFMT_RGBIF_65K);
}

static inline void transfer_command(uint8_t value) {
    gpio_write(LCD_DC_PIN, false);
    gpio_write(LCD_CS_PIN, false);
    spi_transfer_blocking(&value, 1);
    gpio_write(LCD_CS_PIN, true);
}

static inline void transfer_data8(uint8_t value) {
    gpio_write(LCD_DC_PIN, true);
    gpio_write(LCD_CS_PIN, false);
    spi_transfer_blocking(&value, 1);
    gpio_write(LCD_CS_PIN, true);
}

static inline void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    transfer_command(ST7789_COMMAND_CASET);
    transfer_data8((x0>>8)&0xFF);
    transfer_data8((x0>>0)&0xFF);
    transfer_data8((x1>>8)&0xFF);
    transfer_data8((x1>>0)&0xFF);

    transfer_command(ST7789_COMMAND_RASET);
    transfer_data8((y0>>8)&0xFF);
    transfer_data8((y0>>0)&0xFF);
    transfer_data8((y1>>8)&0xFF);
    transfer_data8((y1>>0)&0xFF);

    transfer_command(ST7789_COMMAND_RAMWR);
}

void st7789_init(uint32_t clock_rate) {

    init_st7789_registers(&st7789_regs);

    spi_init(clock_rate);

    gpio_output_init(LCD_RST_PIN, true);
    gpio_output_init(LCD_DC_PIN, false);
    gpio_output_init(LCD_BL_PIN, true);
    gpio_output_init(LCD_CS_PIN, true);

    // INVON
    transfer_command(ST7789_COMMAND_INVON);

    // MADCTRL
    transfer_command(ST7789_COMMAND_MADCTL);
    transfer_data8(st7789_regs.MADCTRL);

    // COLMOD
    transfer_command(ST7789_COMMAND_COLMOD);
    transfer_data8(st7789_regs.COLMOD);

    // SLPOUT
    transfer_command(ST7789_COMMAND_SLPOUT);
    transfer_command(ST7789_COMMAND_DISPON);
}


void st7789_reset(void) {

    gpio_write(LCD_RST_PIN, false);
    sleep_ms(100);
    gpio_write(LCD_RST_PIN, true);
    sleep_ms(100);
}

int st7789_transfer_region_blocking_rgb565(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    (void)len;

    size_t dx, dy, count;
    dx = x1 - x0 + 1;
    dy = y1 - y0 + 1;
    count = dx * sizeof(uint16_t); 
    const uint16_t *image = (mgc_color_t*)buffer;

    set_window(x0, y0, x1, y1);
    gpio_write(LCD_DC_PIN, true);
    gpio_write(LCD_CS_PIN, false);
    for (size_t j = 0; j < dy; j++ ) {
        spi_transfer_blocking((uint8_t*)&image[j * dx], count);
    }
    gpio_write(LCD_CS_PIN, true);

    return 0;
}

int st7789_transfer_full_region_blocking_rgb565(uint8_t *buffer, size_t len) {

    if ( ( LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t) ) != len ) {
        return -1;
    }

    set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    gpio_write(LCD_DC_PIN, true);
    gpio_write(LCD_CS_PIN, false);
    spi_transfer_blocking(buffer, len);
    gpio_write(LCD_CS_PIN, true);

    return 0;
}

