cmake_minimum_required(VERSION 3.16)

project(mgc C)
set(CMAKE_C_STANDARD 11)

set(MGC_SOURCES
    src/mgc/components/sprite.c
    src/mgc/components/hitbox.c
    src/mgc/components/tilemap.c
    src/mgc/components/label.c
    src/mgc/components/textblock.c
    src/mgc/components/dialoguebox.c
    src/mgc/components/rect.c
    src/mgc/components/selectbox.c
    src/mgc/render/camera.c
    src/mgc/render/pixelbuffer.c
    src/mgc/render/framebuffer.c
    src/mgc/font/font.c
    src/mgc/font/encoding.c
    src/mgc/detector/maphit.c
    src/mgc/detector/sprhit.c
    src/mgc/sequencer/dlgflow.c
    src/mgc/sequencer/talkflow.c
    src/mgc/utils/jumpctrl/jumpctrl.c
)

#add_definitions(-DMGC_USE_CUSTOM_MGC_CONF_H)
add_library(mgc STATIC ${MGC_SOURCES})
target_include_directories(mgc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/devices
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)
