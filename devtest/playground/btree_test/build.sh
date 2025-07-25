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

python3 ${TILESET_GEN}      ${COMMON_PATH}/assets/tileset/tileset_map_elements.bmp --count 100 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ${COMMON_PATH}/assets/tileset/tileset_player.bmp --count 25 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ${COMMON_PATH}/assets/tileset/tileset_blue.bmp --count 25 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ${COMMON_PATH}/assets/tileset/tileset_fish.bmp --count 25 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ${COMMON_PATH}/assets/tileset/tileset_items.bmp --count 16 --width 8 --height 8 --dir ./resources/generated/tileset
python3 ${MAP_GEN}          ${COMMON_PATH}/assets/map/map_main_layer_01.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ${COMMON_PATH}/assets/map/map_bg_layer_01.csv --compression runlength --dir ./resources/generated/map
python3 ${TALKSCRIPT_GEN}   ${COMMON_PATH}/assets/talkscript/test_talkscript.yml --dir ./resources/generated/talkscript
python3 ${FONT_GEN}         ${FONTS_PATH}/k8x12/k8x12.bdf  --subset ${COMMON_PATH}/assets/font/k8x12_subset.txt  --dir ./resources/generated/font
python3 ${BTREE_GEN}        ${COMMON_PATH}/assets/btree/test_btree.yaml --dir ./resources/generated/btree
python3 ${BTREE_GEN}        ${COMMON_PATH}/assets/btree/btree_chase.yml --dir ./resources/generated/btree


BUILD_DIR=${BUILD_DIR:-build}
JOBS=${JOBS:-$(nproc)}

cmake -S . -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                           -DMGC_USE_DEFAULT_DIGITAL_GAMEPAD=ON \
                           -DMGC_PIXELBUF_ORDER=1

cmake --build "$BUILD_DIR" --parallel "$JOBS"
