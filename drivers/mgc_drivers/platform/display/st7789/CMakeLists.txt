set(ST7789_SRC
    "st7789.c"
)

if(MGC_USE_RP2040)
    list(APPEND ST7789_SRC
        "ports/rp2040/st7789_rp2040.c"
    )
endif()

add_library(mgc_display_st7789 STATIC ${ST7789_SRC})


set(MGC_DRIVERS_ST7789_WIDTH     "" CACHE STRING "Override: ST7789 display width")
set(MGC_DRIVERS_ST7789_HEIGHT    "" CACHE STRING "Override: ST7789 display height")


foreach(OPTION
    MGC_DRIVERS_ST7789_WIDTH
    MGC_DRIVERS_ST7789_HEIGHT
)
    if(NOT "${${OPTION}}" STREQUAL "")
        target_compile_definitions(mgc_drivers_st7789 PUBLIC ${OPTION}=${${OPTION}})
    endif()
endforeach()


if(MGC_USE_RP2040)
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

endif()


target_include_directories(mgc_display_st7789 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}
)

if(MGC_ENABLE_CPP_INTERFACE)
    target_include_directories(mgc_display_st7789 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    )
endif()


if(MGC_USE_RP2040)
    target_link_libraries(mgc_display_st7789 
        pico_stdlib
        hardware_spi
        hardware_irq
        hardware_dma
    )
endif()

