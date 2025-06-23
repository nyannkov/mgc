set(MGC_C_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/components/dialoguebox.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/hitbox.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/label.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/rect.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/selectbox.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/sprite.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/textblock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/tilemap.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/detector/sprhit.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/detector/maphit.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/font/encoding.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/font/font.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/render/camera.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/render/framebuffer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/render/pixelbuffer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/sequencer/dlgflow.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/sequencer/talkflow.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/jumpctrl/jumpctrl.c"
)

add_library(mgc_c STATIC ${MGC_C_SOURCES})


option(MGC_COLOR_NOSWAP "Do not swap byte order when storing 16-bit color data" OFF)
if(MGC_COLOR_NOSWAP)
    target_compile_definitions(mgc_c PUBLIC MGC_PIXELBUF_NOSWAP)
endif()

set(MGC_DRAW_BUFFER_DIRECTION 1 CACHE STRING "0 for vertical (Y) order, non-zero for horizontal (X) order")
target_compile_definitions(mgc_c PUBLIC MGC_PIXELBUF_ORDER=${MGC_DRAW_BUFFER_DIRECTION})


target_include_directories(mgc_c PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_compile_options(mgc_c PRIVATE ${MGC_C_COMPILE_FLAGS_LIST})

