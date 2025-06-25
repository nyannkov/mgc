set(ST7789_SRC
    "st7789.c"
)

if(MGC_USE_RP2040)
    list(APPEND ST7789_SRC
        "ports/rp2040/st7789_rp2040.c"
    )
endif()

add_library(mgc_display_st7789 STATIC ${ST7789_SRC})

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
    )
endif()

