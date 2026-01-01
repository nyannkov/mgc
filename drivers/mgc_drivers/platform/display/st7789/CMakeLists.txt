add_library(mgc_display_st7789 STATIC 
    st7789.c
)

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
    add_subdirectory(ports/rp2040)
endif()

target_include_directories(mgc_display_st7789 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    ${MGC_SRC_DIR}
)

target_link_libraries(mgc_display_st7789 
    st7789_port
)

