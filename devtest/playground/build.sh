#!/bin/sh
set -e  # Stop on error

if [ -n "$PICO_SDK_PATH" ]; then
	# RP2040 toolchain (GCC for Cortex-M0+)
	TOOLCHAIN_FILE="${PICO_SDK_PATH}/cmake/preload/toolchains/pico_arm_cortex_m0plus_gcc.cmake"
else
	echo "Error: PICO_SDK_PATH is not set and TOOLCHAIN_FILE is not provided."
	exit 1
fi



TILESET_GEN="../../tools/tileset_gen/tileset_gen.py"
MAP_GEN="../../tools/map_gen/map_gen.py"
FONT_GEN="../../tools/font_gen/font_gen.py"
TALKSCRIPT_GEN="../../tools/talkscript_gen/talkscript_gen.py"

python3 ${TILESET_GEN} ../common/assets/tileset/tileset_map_elements.bmp --count 100 --dir ./resources/generated/tileset
python3 ${TILESET_GEN} ../common/assets/tileset/tileset_player.bmp --count 25 --dir ./resources/generated/tileset
python3 ${MAP_GEN}     ../common/assets/map/map_01.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}     ../common/assets/map/map_01_fg.csv --compression runlength --dir ./resources/generated/map


BUILD_DIR=${BUILD_DIR:-build}
JOBS=${JOBS:-$(nproc)}

cmake -S . -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                           -DMGC_USE_DEFAULT_DIGITAL_GAMEPAD=ON \
                           -DMGC_PIXELBUF_ORDER=1

cmake --build "$BUILD_DIR" --parallel "$JOBS"
