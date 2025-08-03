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
TILE_SPLITTER="${TOOLS_PATH}/anim_gen/splitter.py"


python3 ${TILE_SPLITTER}    ${COMMON_PATH}/assets/tileset/tileset_player.bmp --width 16 --height 16 --dir ./resources/anim/player
python3 ${ANIM_GEN}   ./resources/anim/player/anim_player.yml  --dir ./resources/generated/anim


BUILD_DIR=${BUILD_DIR:-build}
JOBS=${JOBS:-$(nproc)}

cmake -S . -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                           -DMGC_USE_DEFAULT_DIGITAL_GAMEPAD=ON \
                           -DMGC_PIXELBUF_ORDER=1

cmake --build "$BUILD_DIR" --parallel "$JOBS"
