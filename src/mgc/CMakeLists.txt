set(MGC_C_COMPILE_FLAGS "-Wall -Wextra -Werror -O2" CACHE STRING "Compile flags for mgc_c library")
separate_arguments(MGC_C_COMPILE_FLAGS_LIST UNIX_COMMAND "${MGC_C_COMPILE_FLAGS}")

file(GLOB_RECURSE MGC_C_SOURCES CONFIGURE_DEPENDS
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
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/jumpctrl.c"
)

add_library(mgc_c STATIC ${MGC_C_SOURCES})

target_include_directories(mgc_c PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_compile_options(mgc_c PRIVATE ${MGC_C_COMPILE_FLAGS_LIST})

