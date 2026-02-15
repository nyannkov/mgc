#!/bin/sh
set -e  # Stop on error

MGC_PATH=${MGC_PATH:-../../}
MGC_ABS_PATH=$(realpath ${MGC_PATH})

SIM="${SIM:-OFF}"

if [ "$SIM" = "ON" ]; then
    USE_RP2040=OFF
    MGC_ENABLE_DISPLAY=OFF
    SCENE_POOL_SIZE=4096
    NOSWAP=1
    BUILD_DIR=${BUILD_DIR:-sim_build}
    EXEC_NAME=test_game
else
    USE_RP2040=ON
    MGC_ENABLE_DISPLAY=ON
    SCENE_POOL_SIZE=8192
    NOSWAP=0
    BUILD_DIR=${BUILD_DIR:-build}
    EXEC_NAME=test_game.elf
    if [ -n "$PICO_SDK_PATH" ]; then
        # RP2040 toolchain (GCC for Cortex-M0+)
        TOOLCHAIN_FILE="${PICO_SDK_PATH}/cmake/preload/toolchains/pico_arm_cortex_m0plus_gcc.cmake"
    else
        echo "Error: PICO_SDK_PATH is not set and TOOLCHAIN_FILE is not provided."
        exit 1
    fi
fi

#. ./resource_gen.sh

JOBS=${JOBS:-$(nproc)}

cmake -S . -B "$BUILD_DIR" -DEXEC_NAME="$EXEC_NAME" \
                           -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                           -DMGC_PIXELBUF_NOSWAP="$NOSWAP" \
                           -DMGC_PIXELBUF_ORDER=1 \
                           -DMGC_MAP_TILESET_INDEX_OFFSET=1 \
                           -DMGC_USE_RP2040="$USE_RP2040" \
                           -DMGC_ENABLE_DISPLAY="$MGC_ENABLE_DISPLAY" \
                           -DSIM_BUILD="$SIM" \
                           -DMGC_ABS_PATH="$MGC_ABS_PATH"

cmake --build "$BUILD_DIR" --parallel "$JOBS"


