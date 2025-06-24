set(ST7789_SRC
    "ST7789.c"
)

if(MGC_USE_RP2040)
    list(APPEND ST7789_SRC
        "ports/rp2040/ST7789_rp2040.c"
    )
endif()

add_library(mgc_display_st7789 STATIC ${ST7789_SRC})

set(MGC_ST7789_SPI_PORT "" CACHE STRING "Override: SPI port to use (e.g., spi0, spi1)")
set(MGC_ST7789_SCAN_DIR "" CACHE STRING "Override: Scan direction (e.g., HORIZONTAL, VERTICAL)")

set(MGC_ST7789_LCD_RST_PIN "" CACHE STRING "Override: GPIO pin for LCD reset")
set(MGC_ST7789_LCD_DC_PIN "" CACHE STRING "Override: GPIO pin for LCD DC")
set(MGC_ST7789_LCD_BL_PIN "" CACHE STRING "Override: GPIO pin for LCD backlight")
set(MGC_ST7789_LCD_CS_PIN "" CACHE STRING "Override: GPIO pin for LCD CS")
set(MGC_ST7789_LCD_CLK_PIN "" CACHE STRING "Override: GPIO pin for LCD CLK")
set(MGC_ST7789_LCD_MOSI_PIN "" CACHE STRING "Override: GPIO pin for LCD MOSI")

if(NOT "${MGC_ST7789_SPI_PORT}" STREQUAL "")
	target_compile_definitions(mgc_drivers_st7789 PUBLIC
		MGC_DRIVERS_ST7789_RP2040_SPI_PORT=${MGC_ST7789_SPI_PORT})
endif()
if(NOT "${MGC_ST7789_SCAN_DIR}" STREQUAL "")
	target_compile_definitions(mgc_drivers_st7789 PUBLIC
		MGC_DRIVERS_ST7789_RP2040_SCAN_DIR=${MGC_ST7789_SCAN_DIR})
endif()
foreach(pin IN ITEMS
	LCD_RST_PIN LCD_DC_PIN LCD_BL_PIN LCD_CS_PIN LCD_CLK_PIN LCD_MOSI_PIN)
	string(CONCAT pin_var "MGC_ST7789_" ${pin})
	if(NOT "${${pin_var}}" STREQUAL "")
		string(CONCAT def_name "MGC_DRIVERS_ST7789_RP2040_" ${pin})
		target_compile_definitions(mgc_drivers_st7789 PUBLIC ${def_name}=${${pin_var}})
	endif()
endforeach()

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
        hardware_pwm
    )
endif()

