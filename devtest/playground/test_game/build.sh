#!/bin/sh
set -e  # Stop on error

if [ -n "$PICO_SDK_PATH" ]; then
    # RP2040 toolchain (GCC for Cortex-M0+)
    TOOLCHAIN_FILE="${PICO_SDK_PATH}/cmake/preload/toolchains/pico_arm_cortex_m0plus_gcc.cmake"
else
    echo "Error: PICO_SDK_PATH is not set and TOOLCHAIN_FILE is not provided."
    exit 1
fi

TOOLS_PATH="../../../tools"
COMMON_PATH="../../common"
FONTS_PATH="../../../assets/fonts"


TILESET_GEN="${TOOLS_PATH}/tileset_gen/tileset_gen.py"
MAP_GEN="${TOOLS_PATH}/map_gen/map_gen.py"
FONT_GEN="${TOOLS_PATH}/font_gen/font_gen.py"
TALKSCRIPT_GEN="${TOOLS_PATH}/talkscript_gen/talkscript_gen.py"
BTREE_GEN="${TOOLS_PATH}/btree_gen/btree_gen.py"
ANIM_GEN="${TOOLS_PATH}/anim_gen/anim_gen.py"
ANIM_FRAME_SPLITTER="${TOOLS_PATH}/anim_frame_splitter/anim_frame_splitter.py"

#python3 ${ANIM_FRAME_SPLITTER}    ./resources/tileset/tileset_skyfish.bmp --width 16 --height 16 --dir ./resources/anim/enemy/skyfish
#python3 ${ANIM_FRAME_SPLITTER}    ./resources/tileset/tileset_player.bmp --width 16 --height 16 --dir ./resources/anim/player

python3 ${ANIM_GEN}         ./resources/anim/player/anim_player.yml  --dir ./resources/generated/anim/player
python3 ${ANIM_GEN}         ./resources/anim/enemy/skyfish/anim_skyfish.yml  --dir ./resources/generated/anim/enemy/skyfish

python3 ${TILESET_GEN}      ./resources/tileset/tileset_map_elements.bmp --count 40 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_blue.bmp --count 25 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_items.bmp --count 16 --width 8 --height 8 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_attack.bmp --count 2 --width 8 --height 16 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_static_objects.bmp --count 5 --dir ./resources/generated/tileset

python3 ${MAP_GEN}          ./resources/map/map_scene_001_main.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/map/map_scene_001_bg1.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/map/map_scene_001_bg2.csv --compression runlength --dir ./resources/generated/map

python3 ${TALKSCRIPT_GEN}   ./resources/talkscript/test_talkscript.yml --dir ./resources/generated/talkscript

python3 ${BTREE_GEN}        ./resources/btree/test_btree.yaml --dir ./resources/generated/btree
python3 ${BTREE_GEN}        ./resources/btree/btree_chase.yml --dir ./resources/generated/btree
python3 ${FONT_GEN}         ${FONTS_PATH}/k8x12/k8x12.bdf  --subset  ./resources/font/k8x12_subset.txt  --dir ./resources/generated/font
python3 ${FONT_GEN}         ${FONTS_PATH}/misaki/misaki_gothic.bdf  --subset  ./resources/font/misaki_subset.txt  --dir ./resources/generated/font


BUILD_DIR=${BUILD_DIR:-build}
JOBS=${JOBS:-$(nproc)}

cmake -S . -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                           -DMGC_USE_DEFAULT_DIGITAL_GAMEPAD=ON \
                           -DMGC_PIXELBUF_ORDER=1 \
                           -DMGC_MAP_TILESET_INDEX_OFFSET=1

cmake --build "$BUILD_DIR" --parallel "$JOBS"
