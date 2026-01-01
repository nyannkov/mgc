add_library(st7789_port STATIC 
    st7789_rp2040.c
)

set(MGC_DRIVERS_ST7789_RP2040_SPI_PORT     "" CACHE STRING "Override: ST7789 SPI port for RP2040 (e.g., spi0 or spi1)")
set(MGC_DRIVERS_ST7789_RP2040_SPI_CLOCK_RATE     "" CACHE STRING "Override: ST7789 SPI default clock rate for RP2040 (unit: Hz)")
set(MGC_DRIVERS_ST7789_RP2040_DMA_IRQ_PRI  "" CACHE STRING "Override: ST7789 DMA IRQ priority for RP2040 (0-255)")
set(MGC_DRIVERS_ST7789_RP2040_PIN_RST  "" CACHE STRING "Override: ST7789 reset pin")
set(MGC_DRIVERS_ST7789_RP2040_PIN_DC   "" CACHE STRING "Override: ST7789 DC pin")
set(MGC_DRIVERS_ST7789_RP2040_PIN_BL   "" CACHE STRING "Override: ST7789 backlight pin")
set(MGC_DRIVERS_ST7789_RP2040_PIN_CS   "" CACHE STRING "Override: ST7789 CS pin")
set(MGC_DRIVERS_ST7789_RP2040_PIN_CLK  "" CACHE STRING "Override: ST7789 CLK pin")
set(MGC_DRIVERS_ST7789_RP2040_PIN_MOSI "" CACHE STRING "Override: ST7789 MOSI pin")

foreach(RP2040_OPT
    MGC_DRIVERS_ST7789_RP2040_SPI_PORT
    MGC_DRIVERS_ST7789_RP2040_SPI_CLOCK_RATE
    MGC_DRIVERS_ST7789_RP2040_DMA_IRQ_PRI
    MGC_DRIVERS_ST7789_RP2040_PIN_RST
    MGC_DRIVERS_ST7789_RP2040_PIN_DC
    MGC_DRIVERS_ST7789_RP2040_PIN_BL
    MGC_DRIVERS_ST7789_RP2040_PIN_CS
    MGC_DRIVERS_ST7789_RP2040_PIN_CLK
    MGC_DRIVERS_ST7789_RP2040_PIN_MOSI
)
    if(NOT "${${RP2040_OPT}}" STREQUAL "")
        target_compile_definitions(mgc_drivers_st7789 PUBLIC ${RP2040_OPT}=${${RP2040_OPT}})
    endif()
endforeach()

target_include_directories(st7789_port PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}/mgc_drivers/platform/display/st7789
)

target_link_libraries(st7789_port 
    pico_stdlib
    hardware_spi
    hardware_irq
    hardware_dma
    hardware_sync
)

